//
// Created by marhc on 16/05/2021.
//

#ifndef ENTREGA2_VEHICLE_H
#define ENTREGA2_VEHICLE_H

#include <iostream>
#include <string>

class Vehicle {
    std::string licensePlate;
    double range; // Meters
public:
    static double maxRange;
    Vehicle();
    explicit Vehicle(std::string licensePlate);

    std::string getLicensePlate() const;
    double getRange() const;
    double getMaxRange() const;
    void setRange(double range);

    void print();

    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);
    friend std::istream& operator>>(std::istream& is, Vehicle& vehicle);
};


#endif //ENTREGA2_VEHICLE_H
