//
//  main.cpp
//  AdapterPattern
//
//  Created by Anussha on 08/09/26.
//

#include <iostream>
#include <string>
#include <sstream>

class TemperatureSensor{
public:
    virtual double readCelcius() const = 0;
    virtual ~TemperatureSensor() = default;
};

// Legacy class we can't modify — Fahrenheit as a string
class LegacyFahrenheitSensor {
public:
    std::string getTemperature() const {
        return "98.6F";
    }
};

class FahrenheitToCelsiusAdapter : public TemperatureSensor {
private:
    LegacyFahrenheitSensor legacySensor;
public:
    explicit FahrenheitToCelsiusAdapter(LegacyFahrenheitSensor sensor)
            : legacySensor(sensor) {}
    
    double readCelcius() const override {
        std::string raw = legacySensor.getTemperature();  // "98.6F"
        std::cout<<"raw:"<<raw<<"\n";
        std::string numericPart = raw.substr(0, raw.size() - 1); // "98.6"
        std::cout<<"numericPart:"<<numericPart<<"\n";
        double fahrenheit = std::stod(numericPart);
        std::cout<<"fahrenheit:"<<fahrenheit<<"\n";
        return (fahrenheit - 32) * 5.0 / 9.0;
    }
};

void printReport(const TemperatureSensor& sensor) {
    std::cout << "Current temperature: " << sensor.readCelcius() << "°C" << std::endl;
}

int main() {
    LegacyFahrenheitSensor legacy;
    FahrenheitToCelsiusAdapter adapter(legacy);

    printReport(adapter);

    return 0;
}
