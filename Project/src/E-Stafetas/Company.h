//
// Created by marhc on 17/05/2021.
//

#ifndef ENTREGA2_COMPANY_H
#define ENTREGA2_COMPANY_H

#include <sstream>

#include "Client.h"
#include "Order.h"
#include "Driver.h"
#include "Vehicle.h"
#include "../Algorithms/Graph.h"

template <class T>
class ItemNotFoundException {
    std::string message;
public:
    ItemNotFoundException(std::string type, T data, std::string funcName) {
        std::stringstream ss;
        ss << "Error in " << funcName << ": " << type << " " << data << " was not found";
        message = ss.str();
    }
    const string &getMessage() const {
        return message;
    }
};

class InvalidNodeException {
    std::string message;
public:
    InvalidNodeException(int id) {
        std::stringstream ss;
        ss << "Node " << id << " cannot be chosen for pickup or delivery point\n";
        message = ss.str();
    }
    const string &getMessage() const {
        return message;
    }
};


class Company {
    std::vector<Driver*> crew;
    std::vector<Order*> orders;
    std::vector<Vehicle*> fleet;
    std::vector<Client*> clients;
    std::vector<Order*> currentOrders;
    Node* garage;
public:
    Company();
    Company(std::vector<Driver*> d, std::vector<Client*> c, std::vector<Order*> o, std::vector<Vehicle*> v, Node* garage);

    const vector<Driver*> &getCrew() const;
    void setCrew(const vector<Driver*> &crew);
    const vector<Order*> &getOrders() const;
    void setOrders(const vector<Order*> &orders);
    const vector<Vehicle*> &getFleet() const;
    void setFleet(const vector<Vehicle*> &fleet);
    const vector<Client*> &getClients() const;
    void setClients(const vector<Client*> &clients);
    const vector<Order *> &getCurrentOrders() const;
    void setCurrentOrders(const vector<Order *> &currentOrders);
    Node *getGarage() const;
    void setGarage(Node *garage);

    Client* findClient(int id);
    Order* findOrder(int id);
    Vehicle* findVehicle(std::string licensePlate);
    Driver* findDriver(int id);

    bool addVehicle(Vehicle* vehicle);
    void addDriver(Driver* driver);
    void addOrder(Order* order);
    void addClient(Client* client);

    bool removeVehicle(std::string licensePlate);
    bool removeClient(int id);
    bool removeOldOrder(int id);
    bool removeDriver(int id);
    bool removeCurrentOrder(int id);

    void listClientsName(std::string name);
    void listDriversName(std::string name);
    void listDrivers();
    void listPastOrders();
    void listCurrentOrders();
    void listVehicles();
    void listClients();
    void listOrdersClients(int id);


    void assignOrdersParameters(Graph& graph);
    void makeRoutes(Graph& graph);
};


#endif //ENTREGA2_COMPANY_H
