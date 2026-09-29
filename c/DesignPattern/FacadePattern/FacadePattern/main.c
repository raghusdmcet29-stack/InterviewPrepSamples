//
//  main.c
//  FacadePattern
//
//  Created by Anussha on 08/09/26.
//
#include <stdio.h>

// Subsystems — plain structs, no state needed for this demo, just behavior via free functions

typedef struct {
    int placeholder; // no real state needed for this demo
} Amplifier;

void amplifierOn(Amplifier* self) { printf("Amplifier on\n"); }
void amplifierSetVolume(Amplifier* self, int level) { printf("Amplifier volume set to %d\n", level); }
void amplifierOff(Amplifier* self) { printf("Amplifier off\n"); }

typedef struct {
    int placeholder;
} DVDPlayer;

void dvdOn(DVDPlayer* self) { printf("DVD Player on\n"); }
void dvdPlay(DVDPlayer* self, const char* movie) { printf("DVD Player playing \"%s\"\n", movie); }
void dvdStop(DVDPlayer* self) { printf("DVD Player stopped\n"); }
void dvdOff(DVDPlayer* self) { printf("DVD Player off\n"); }

typedef struct {
    int placeholder;
} Projector;

void projectorOn(Projector* self) { printf("Projector on\n"); }
void projectorSetInput(Projector* self, const char* source) { printf("Projector input set to %s\n", source); }
void projectorOff(Projector* self) { printf("Projector off\n"); }

typedef struct {
    Amplifier* amp;
    DVDPlayer* dvd;
    Projector* projector;
} HomeTheaterFacade;

HomeTheaterFacade makeHomeTheaterFacade(Amplifier* amp, DVDPlayer* dvd, Projector* projector) {
    HomeTheaterFacade facade;
    facade.amp = amp;
    facade.dvd = dvd;
    facade.projector = projector;
    return facade;
}

void homeTheaterWatchMovie(HomeTheaterFacade* self, const char* movie) {
    printf("--- Setting up movie night ---\n");
    amplifierOn(self->amp);
    amplifierSetVolume(self->amp, 5);
    projectorOn(self->projector);
    projectorSetInput(self->projector, "DVD");
    dvdOn(self->dvd);
    dvdPlay(self->dvd, movie);
}

void homeTheaterEndMovie(HomeTheaterFacade* self) {
    printf("--- Shutting down movie night ---\n");
    dvdStop(self->dvd);
    dvdOff(self->dvd);
    projectorOff(self->projector);
    amplifierOff(self->amp);
}

int main(void) {
    Amplifier amp;
    DVDPlayer dvd;
    Projector projector;

    HomeTheaterFacade homeTheater = makeHomeTheaterFacade(&amp, &dvd, &projector);

    homeTheaterWatchMovie(&homeTheater, "Inception");
    printf("\n");
    homeTheaterEndMovie(&homeTheater);

    return 0;
}
