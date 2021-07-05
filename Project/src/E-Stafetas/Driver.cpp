//
// Created by marhc on 16/05/2021.
//

#include "Driver.h"

int Driver::currentId = 1;

Driver::Driver() : id(currentId) {
    Driver::currentId++;
}

Driver::Driver(std::string name, float salary) : name(std::move(name)), id(Driver::currentId), salary(salary) {
    Driver::currentId++;
}

std::string Driver::getName() const {
    return name;
}

int Driver::getId() const {
    return id;
}

float Driver::getSalary() const {
    return salary;
}

void Driver::setSalary(float salary) {
    Driver::salary = salary;
}

const std::vector<int> &Driver::getRoute() const {
    return route;
}

void Driver::setRoute(const std::vector<int> &route) {
    Driver::route = route;
}

void Driver::printRoute() {
    std::cout << "ID:" << id << " Name:" << name << " Route: ";
    for (int id : route)
        std::cout << id << " ";
    std::cout << std::endl;
}

void Driver::print() {
    std::cout << "ID:" << this->getId() << " Name:" << this->getName();
    std::cout << " Salary:" << this->getSalary() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Driver& driver) {
    os << driver.name << "\n" << driver.salary << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Driver& driver) {
    std::getline(is, driver.name);
    is >> driver.salary;
    is.ignore(20, '\n');
    return is;
}
