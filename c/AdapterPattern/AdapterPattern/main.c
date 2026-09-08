//
//  main.c
//  AdapterPattern
//
//  Created by Anussha on 08/09/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Target interface — struct of function pointers, same pattern as C's
// Observer/Command/State/Decorator projects

typedef struct TemperatureSensor {
    double (*readCelsius)(struct TemperatureSensor* self);
    void* data; // opaque pointer to whatever concrete data this sensor needs
} TemperatureSensor;


// Legacy struct we can't modify — Fahrenheit as a string
typedef struct {
    char temperature[16]; // e.g. "98.6F"
} LegacyFahrenheitSensor;


LegacyFahrenheitSensor legacyGetSensor(void) {
    LegacyFahrenheitSensor sensor;
    strcpy(sensor.temperature, "98.6F");
    return sensor;
}

double fahrenheitAdapterReadCelsius(TemperatureSensor* self) {
    LegacyFahrenheitSensor* legacy = (LegacyFahrenheitSensor*)self->data;

    char numericPart[16];
    size_t len = strlen(legacy->temperature);
    strncpy(numericPart, legacy->temperature, len - 1); // drop trailing 'F'
    numericPart[len - 1] = '\0';

    double fahrenheit = atof(numericPart);
    return (fahrenheit - 32) * 5.0 / 9.0;
}

TemperatureSensor makeFahrenheitAdapter(LegacyFahrenheitSensor* legacySensor) {
    TemperatureSensor adapter;
    adapter.readCelsius = fahrenheitAdapterReadCelsius;
    adapter.data = legacySensor; // opaque pointer to the legacy struct
    return adapter;
}

void printReport(TemperatureSensor* sensor) {
    double celsius = sensor->readCelsius(sensor);
    printf("Current temperature: %.1f°C\n", celsius);
}

int main(void) {
    LegacyFahrenheitSensor legacy = legacyGetSensor();
    TemperatureSensor adapter = makeFahrenheitAdapter(&legacy);

    printReport(&adapter);

    return 0;
}
