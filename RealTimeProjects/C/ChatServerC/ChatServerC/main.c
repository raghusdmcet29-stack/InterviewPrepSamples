//
//  main.c
//  ChatServerC
//
//  Created by Anussha on 17/11/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#define MAX_CLIENTS 64



void broadcast(struct pollfd fds[], char rooms[][32], int nfds, int senderIdx, const char *msg, ssize_t len) {
    for (int i = 1; i < nfds; i++) {
        if (i != senderIdx && strcmp(rooms[i], rooms[senderIdx]) == 0) {
            write(fds[i].fd, msg, len);
        }
    }
}

int main(void) {
    signal(SIGPIPE, SIG_IGN);
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) {
        perror("socket");
        return 1;
    }
    printf("Listening socket created: %d\n", listenFd);
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(listenFd, (struct sockaddr *)&addr, sizeof addr) < 0) {
        perror("bind");
        return 1;
    }
    
    if (listen(listenFd, 10) < 0) {
        perror("listen");
        return 1;
    }
    
    printf("Listening on port 8080\n");
    
    struct pollfd fds[MAX_CLIENTS + 1];
    char rooms[MAX_CLIENTS + 1][32];
    int nfds = 1;
    fds[0].fd = listenFd;
    fds[0].events = POLLIN;
    
    while (1) {
        int ready = poll(fds, nfds, -1);
        if (ready < 0) {
            perror("poll");
            return 1;
        }
        
        if (fds[0].revents & POLLIN) {
            int clientFd = accept(listenFd, NULL, NULL);
            if (clientFd < 0) {
                perror("accept");
                continue;
            }
            if (nfds >= MAX_CLIENTS + 1) {
                close(clientFd);
                continue;
            }
            fds[nfds].fd = clientFd;
            fds[nfds].events = POLLIN;
            rooms[nfds][0] = '\0';
            nfds++;
            printf("client connected (fd %d), total clients: %d\n",
                   clientFd, nfds - 1);
        }
        
        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                char buf[256];
                ssize_t n = read(fds[i].fd, buf, sizeof buf - 1);
                if (n <= 0) {
                    printf("client fd %d disconnected\n", fds[i].fd);
                    close(fds[i].fd);
                    fds[i] = fds[nfds - 1];
                    snprintf(rooms[i], sizeof rooms[i], "%s", rooms[nfds - 1]);
                    nfds--;
                    i--;
                    continue;
                }
                buf[n] = '\0';
                if (strncmp(buf, "/join", 5) == 0 && (buf[5] == ' ' || buf[5] == '\n' || buf[5] == '\0')) {
                    const char *name = buf + 5;
                    while (*name == ' ') name++;
                    if (*name == '\0' || *name == '\n' || *name == '\r') {
                        const char *usage = "usage: /join <room name>\n";
                        write(fds[i].fd, usage, strlen(usage));
                        continue;
                    }
                    snprintf(rooms[i], sizeof rooms[i], "%s", name);
                    rooms[i][strcspn(rooms[i], "\r\n")] = '\0';
                    printf("fd %d joined room %s\n", fds[i].fd, rooms[i]);
                    continue;
                }
                if (strcmp(buf, "/leave\n") == 0 || strcmp(buf, "/leave\r\n") == 0) {
                    if (rooms[i][0] == '\0') {
                        const char *msg = "you are not in a room\n";
                        write(fds[i].fd, msg, strlen(msg));
                        continue;
                    }
                    printf("fd %d left room %s\n", fds[i].fd, rooms[i]);
                    rooms[i][0] = '\0';
                    const char *msg = "you left the room\n";
                    write(fds[i].fd, msg, strlen(msg));
                    continue;
                }
                if (strncmp(buf, "/who", 4) == 0 &&
                    (buf[4] == '\n' || buf[4] == '\r' || buf[4] == '\0')) {
                    if (rooms[i][0] == '\0') {
                        const char *msg = "join a room first with /join <name>\n";
                        write(fds[i].fd, msg, strlen(msg));
                        continue;
                    }
                    char list[512] = "in your room:";
                    for (int j = 1; j < nfds; j++) {
                        if (strcmp(rooms[j], rooms[i]) == 0) {
                            char entry[32];
                            snprintf(entry, sizeof entry, " fd%d", fds[j].fd);
                            strncat(list, entry, sizeof list - strlen(list) - 1);
                        }
                    }
                    strncat(list, "\n", sizeof list - strlen(list) - 1);
                    write(fds[i].fd, list, strlen(list));
                    continue;
                }
                if (rooms[i][0] == '\0') {
                    const char *hint = "join a room first with /join <name>\n";
                    write(fds[i].fd, hint, strlen(hint));
                    continue;
                }                printf("from fd %d: %s", fds[i].fd, buf);
                broadcast(fds, rooms, nfds, i, buf, n);
            }
        }
    }
    
    return 0;
}
