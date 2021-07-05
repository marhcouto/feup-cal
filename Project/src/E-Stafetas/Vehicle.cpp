//
// Created by marhc on 16/05/2021.
//

#include "Vehicle.h"

#include <utility>

double Vehicle::maxRange = 30000;

Vehicle::Vehicle() : range(Vehicle::maxRange) {};

Vehicle::Vehicle(std::string licensePlate) : licensePlate(std::move(licensePlate)), range(Vehicle::maxRange) {};

std::string Vehicle::getLicensePlate() const {
    return licensePlate;
}

double Vehicle::getRange() const {
    return range;
}

double Vehicle::getMaxRange() const {
    return maxRange;
}

void Vehicle::setRange(double range) {
    this->range = range;
}

void Vehicle::print() {
    std::cout << "License Plate:" << this->getLicensePlate() << " MaxRange:" << this->getMaxRange() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
    os << vehicle.licensePlate << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Vehicle& vehicle) {
    is >> vehicle.licensePlate;
    return is;
}

