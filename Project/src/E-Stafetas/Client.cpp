//
// Created by marhc on 16/05/2021.
//

#include "Client.h"

int Client::currentId = 1;

Client::Client() : id(Client::currentId), noOrders(0) {
    Client::currentId++;
}

Client::Client(std::string name) : id(Client::currentId), name(std::move(name)), noOrders(0) {
    Client::currentId++;
}

std::string Client::getName() const {
    return name;
}

int Client::getId() const {
    return id;
}

int Client::getNoOrders() const {
    return noOrders;
}

void Client::incrementOrders() {
    this->noOrders++;
}

void Client::print() {
    std::cout << "ID:" << this->getId() << " Name:" << this->getName() << " Number of orders:" << this->getNoOrders() << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Client& client) {
    os << client.name << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Client& client) {
    std::getline(is, client.name);
    return is;
}