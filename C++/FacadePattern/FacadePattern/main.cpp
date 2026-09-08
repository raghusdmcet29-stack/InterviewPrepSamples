//
//  main.cpp
//  FacadePattern
//
//  Created by Anussha on 08/09/26.
//

#include <iostream>
#include <string>

class Amplifier {
public:
    void on() { std::cout << "Amplifier on" << std::endl; }
    void setVolume(int level) { std::cout << "Amplifier volume set to " << level << std::endl; }
    void off() { std::cout << "Amplifier off" << std::endl; }
};

class DVDPlayer {
public:
    void on() { std::cout << "DVD Player on" << std::endl; }
    void play(const std::string& movie) { std::cout << "DVD Player playing \"" << movie << "\"" << std::endl; }
    void stop() { std::cout << "DVD Player stopped" << std::endl; }
    void off() { std::cout << "DVD Player off" << std::endl; }
};

class Projector {
public:
    void on() { std::cout << "Projector on" << std::endl; }
    void setInput(const std::string& source) { std::cout << "Projector input set to " << source << std::endl; }
    void off() { std::cout << "Projector off" << std::endl; }
};

class HomeTheaterFacade {
private:
    Amplifier& amp;
    DVDPlayer& dvd;
    Projector& projector;

public:
    HomeTheaterFacade(Amplifier& amp, DVDPlayer& dvd, Projector& projector)
        : amp(amp), dvd(dvd), projector(projector) {}

    void watchMovie(const std::string& movie) {
        std::cout << "--- Setting up movie night ---" << std::endl;
        amp.on();
        amp.setVolume(5);
        projector.on();
        projector.setInput("DVD");
        dvd.on();
        dvd.play(movie);
    }

    void endMovie() {
        std::cout << "--- Shutting down movie night ---" << std::endl;
        dvd.stop();
        dvd.off();
        projector.off();
        amp.off();
    }
};

int main() {
    Amplifier amp;
    DVDPlayer dvd;
    Projector projector;

    HomeTheaterFacade homeTheater(amp, dvd, projector);

    homeTheater.watchMovie("Inception");
    std::cout << std::endl;
    homeTheater.endMovie();

    return 0;
}
