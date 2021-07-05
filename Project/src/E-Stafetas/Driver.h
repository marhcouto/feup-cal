//
// Created by marhc on 16/05/2021.
//

#ifndef ENTREGA2_DRIVER_H
#define ENTREGA2_DRIVER_H

#include <iostream>
#include <string>
#include <vector>

class Driver {
    std::string name;
    std::vector<int> route;
    int id;
    float salary;
public:
    static int currentId;

    Driver();
    Driver(std::string name, float salary);

    std::string getName() const;
    int getId() const;
    float getSalary() const;
    const std::vector<int> &getRoute() const;
    void setRoute(const std::vector<int> &route);
    void setSalary(float salary);

    void print();
    void printRoute();

    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);
    friend std::istream& operator>>(std::istream& is, Driver& driver);
};


#endif //ENTREGA2_DRIVER_H
