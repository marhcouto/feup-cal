//
// Created by marhc on 16/05/2021.
//

#ifndef ENTREGA2_CLIENT_H
#define ENTREGA2_CLIENT_H

#include <string>
#include <iostream>
#include <utility>


class Client {
    std::string name;
    int id;
    int noOrders;
public:
    int static currentId;

    Client();
    explicit Client(std::string name);

    std::string getName() const;
    int getId() const;
    int getNoOrders() const;

    void incrementOrders();

    void print();

    friend std::ostream& operator<<(std::ostream& os, const Client& client);
    friend std::istream& operator>>(std::istream& is, Client& client);
};


#endif //ENTREGA2_CLIENT_H
