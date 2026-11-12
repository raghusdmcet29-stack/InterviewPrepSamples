//
//  Server.cpp
//  ChatServerCPP
//
//  Created by Anussha on 10/11/26.
//

#include "Server.hpp"
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

std::vector<pollfd> fds;  // the one real definition
std::map<int, std::string> clientBuffers;  // socket number -> partial text not yet a full line
const size_t MAX_LINE = 1024;  // longest line we allow before disconnecting
std::map<int, std::string> clientOutput;  // socket number -> bytes waiting to be sent
std::map<int, std::string> clientRoom;   // socket number -> room name

int createServerSocket() {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        std::cerr << "socket failed\n";
        return -1;
    }
    return server;
}

bool startListening(int server, int port) {
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        std::cerr << "bind failed\n";
        return false;
    }
    if (listen(server, 5) < 0) {
        std::cerr << "listen failed\n";
        return false;
    }
    
    // Server.cpp, after listen() succeeds
    pollfd listenEntry;
    listenEntry.fd = server;    // the socket we created earlier
    listenEntry.events = POLLIN;   // wake up when a new client is waiting
    listenEntry.revents = 0;       // the OS fills this in later
    fds.push_back(listenEntry);
    
    return true;
}

int acceptClient(int server) {
    int client = accept(server, nullptr, nullptr);
    if (client < 0) {
        std::cerr << "accept failed\n";
    }
    return client;
}

std::string readLine(int client) {
    std::string line;
    char ch;
    while (read(client, &ch, 1) == 1) {
        if (ch == '\n') {
            break;
        }
        line += ch;
    }
    return line;
}

bool sendLine(int client, const std::string& text) {
    std::string data = text + "\n";
    ssize_t sent = write(client, data.c_str(), data.size());
    return sent == static_cast<ssize_t>(data.size());
}

void broadcast(int sender, const std::string& message) {
    std::string data = message + "\n";
    for (size_t j = 1; j < fds.size(); ++j) {
        int fd = fds[j].fd;
        if (fd != sender && clientRoom[fd] == clientRoom[sender]) {
            clientOutput[fd] += data;        // queue it; the flush step sends it
            fds[j].events |= POLLOUT;        // ask poll() to tell us when this socket can take data
        }
    }
}
bool setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0;
}

void flushOutput(int fd) {
    std::string& out = clientOutput[fd];
    while (!out.empty()) {
        ssize_t sent = write(fd, out.data(), out.size());
        if (sent < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return;   // socket is full; keep the rest for later
            }
            return;       // real error; the caller will see it on the next read
        }
        out.erase(0, sent);
    }
}

void runServer(int server) {
    while (true) {
        int ready = poll(fds.data(), static_cast<nfds_t>(fds.size()), -1);
        if (ready < 0) {
            std::cerr << "poll failed\n";
            break;
        }

        // Step 2: is the listening socket ready? Then a client is waiting.
        if (fds[0].revents & POLLIN) {
            int client = acceptClient(server);
            if (client >= 0) {
                if (!setNonBlocking(client)) {
                    std::cerr << "fcntl failed\n";
                    close(client);
                }else {
                    pollfd clientEntry;
                    clientEntry.fd = client;
                    clientEntry.events = POLLIN;   // wake up when this client sends data
                    clientEntry.revents = 0;
                    fds.push_back(clientEntry);
                    std::cout << "client " << client << " connected\n";
                }
            }
        }   // <- listener block ends here
        
        
        // Step 3: client sockets go here
        for (size_t i = 1; i < fds.size(); ++i) {
             if (!(fds[i].revents & (POLLIN | POLLOUT | POLLHUP | POLLERR)))  {
                continue;
            }
            
            // Output: send queued bytes when the socket can take them
            if (fds[i].revents & POLLOUT) {
                flushOutput(fds[i].fd);
            }
            
            if (clientOutput[fds[i].fd].empty()) {
                fds[i].events &= ~POLLOUT;   // nothing left to send; stop watching for writability
            }
            
            int fd = fds[i].fd;
            char buf[1024];
            ssize_t n = read(fd, buf, sizeof(buf));
            
            if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                continue;   // nothing to read yet, not a disconnect
            }
            
            if (n <= 0) {
                // 0 means the client closed its terminal; negative means an error
                std::cout << "client " << fd << " disconnected\n";
                close(fd);
                clientBuffers.erase(fd);   // throw away any unfinished text
                clientRoom.erase(fd);
                fds.erase(fds.begin() + i);
                --i;
                continue;
            }
            
            // Add the new bytes to this client's buffer
            std::string& pending = clientBuffers[fd];
            pending.append(buf, n);
            
            // Print complete lines, rejecting any line longer than MAX_LINE
            bool tooLong = false;
            size_t newline;
            while ((newline = pending.find('\n')) != std::string::npos) {
                if (newline > MAX_LINE) {
                    tooLong = true;
                    break;
                }
                std::string line = pending.substr(0, newline);
                pending.erase(0, newline + 1);
                
                // /leave: go back to having no room
                if (line == "/leave") {
                    clientRoom[fd].clear();
                    clientOutput[fd] += "left the room\n";
                    fds[i].events |= POLLOUT;
                    continue;
                }
                
                
                // 1. /join command
                if (line == "/join" || line.rfind("/join ", 0) == 0) {
                    std::string room = line.size() > 6 ? line.substr(6) : "";
                    size_t space = room.find(' ');
                    if (space != std::string::npos) {
                        room = room.substr(0, space);
                    }
                    if (room.empty()) {
                        clientOutput[fd] += "usage: /join <name>\n";
                        fds[i].events |= POLLOUT;
                        continue;
                    }
                    clientRoom[fd] = room;
                    clientOutput[fd] += "joined room " + room + "\n";
                    fds[i].events |= POLLOUT;
                    continue;
                }

                // 2. Not in a room yet: remind them (this also covers blank lines)
                if (clientRoom[fd].empty()) {
                    clientOutput[fd] += "join a room first with /join <name>\n";
                    fds[i].events |= POLLOUT;
                    continue;
                }

                // 3. In a room, but the line is blank
                if (line.empty()) {
                    clientOutput[fd] += "message is empty, nothing sent\n";
                    fds[i].events |= POLLOUT;
                    continue;
                }
                // 4. Normal chat message
                std::string labeled = "client " + std::to_string(fd) + ": " + line;
                std::cout << labeled << "\n";           // server log
                broadcast(fd, labeled);
            }
            
            // Also reject unfinished text that is already too long
            if (!tooLong && pending.size() > MAX_LINE) {
                tooLong = true;
            }
            
            if (tooLong) {
                std::cout << "client " << fd << " sent a line that is too long, disconnecting\n";
                close(fd);
                clientBuffers.erase(fd);
                clientRoom.erase(fd);
                fds.erase(fds.begin() + i);
                --i;
            }
        }
    }
    std::cout << "runServer came out from pooling: ";

}
