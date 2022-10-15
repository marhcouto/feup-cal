//
// Created by marhc on 17/05/2021.
//

#include "Company.h"

Company::Company() {};

Company::Company(std::vector<Driver*> d, std::vector<Client*> c, std::vector<Order*> o, std::vector<Vehicle*> v, Node* garage) :
orders(o), crew(d), fleet(v), clients(c), garage(garage) {};

const std::vector<Driver*> &Company::getCrew() const {
    return crew;
}

void Company::setCrew(const vector<Driver*> &crew) {
    Company::crew = crew;
}

const std::vector<Order*> &Company::getOrders() const {
    return orders;
}

void Company::setOrders(const vector<Order*> &orders) {
    Company::orders = orders;
}

const std::vector<Vehicle*> &Company::getFleet() const {
    return fleet;
}

void Company::setFleet(const vector<Vehicle*> &fleet) {
    Company::fleet = fleet;
}

const std::vector<Client*> &Company::getClients() const {
    return clients;
}

void Company::setClients(const vector<Client*> &clients) {
    Company::clients = clients;
}

const vector<Order *> &Company::getCurrentOrders() const {
    return currentOrders;
}

void Company::setCurrentOrders(const vector<Order *> &currentOrders) {
    Company::currentOrders = currentOrders;
}

Node *Company::getGarage() const {
    return garage;
}

void Company::setGarage(Node *garage) {
    Company::garage = garage;
}

Client* Company::findClient(int id) {
    for (const auto& client : clients) {
        if (client->getId() == id)
            return client;
    }
    return nullptr;
}

Vehicle* Company::findVehicle(std::string licensePlate) {
    for (const auto& vehicle : fleet) {
        if (vehicle->getLicensePlate() == licensePlate)
            return vehicle;
    }
    return nullptr;
}

Order* Company::findOrder(int id) {
    for (const auto& order : orders) {
        if (order->getOrderId() == id)
            return order;
    }
    return nullptr;
}

Driver* Company::findDriver(int id) {
    for (const auto& driver : crew) {
        if (driver->getId() == id)
            return driver;
    }
    return nullptr;
}

void Company::listClientsName(std::string name) {
    for (const auto& client : clients) {
        if (client->getName() == name)
            client->print();
    }
}

void Company::listDriversName(std::string name) {
    for (const auto& driver : crew) {
        if (driver->getName() == name) {
            driver->print();
        }
    }
}

void Company::listDrivers() {
    for (const auto& driver : crew) {
        driver->print();
    }
}

void Company::listClients() {
    for (const auto& client : clients)
        client->print();
}

void Company::listVehicles() {
    for (const auto& vehicle : fleet)
        vehicle->print();
}

void Company::listPastOrders() {
    for (const auto& order : orders)
        order->print();
}

void Company::listCurrentOrders() {
    for (const auto& order : currentOrders)
        order->print();
}

void Company::listOrdersClients(int id) {
    for (const auto& order : orders) {
        if (order->getClient()->getId() == id)
            order->print();
    }
    for (const auto& order : currentOrders) {
        if (order->getClient()->getId() == id)
            order->print();
    }
}

bool Company::removeClient(int id) {
    for (auto it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->getId() == id) {
            delete *it;
            clients.erase(it);
            return true;
        }
    }
    return false;
}

bool Company::removeDriver(int id) {
    for (auto it = crew.begin(); it != crew.end(); it++) {
        if ((*it)->getId() == id) {
            delete *it;
            crew.erase(it);
            return true;
        }
    }
    return false;
}

bool Company::removeOldOrder(int id) {
    for (auto it = orders.begin(); it != orders.end(); it++) {
        if ((*it)->getOrderId() == id) {
            delete *it;
            orders.erase(it);
            return true;
        }
    }
    return false;
}

bool Company::removeCurrentOrder(int id) {
    for (auto it = currentOrders.begin(); it != currentOrders.end(); it++) {
        if ((*it)->getOrderId() == id) {
            delete *it;
            orders.erase(it);
            return true;
        }
    }
    return false;
}

bool Company::removeVehicle(std::string licensePlate) {
    for (auto it = fleet.begin(); it != fleet.end(); it++) {
        if ((*it)->getLicensePlate() == licensePlate) {
            delete *it;
            fleet.erase(it);
            return true;
        }
    }
    return false;
}

bool Company::addVehicle(Vehicle *vehicle) {
    if (findVehicle(vehicle->getLicensePlate()) != nullptr)
        return false;
    fleet.push_back(vehicle);
    return true;
}

void Company::addDriver(Driver *driver) {
    crew.push_back(driver);
}

void Company::addOrder(Order *order) {
    currentOrders.push_back(order);
    orders.push_back(order);
}

void Company::addClient(Client *client) {
    clients.push_back(client);
}

void Company::assignOrdersParameters(Graph& graph) {
    for (Order* order : orders) {
        Node* n1 = graph.findNode(order->tempPickUpId);
        Node* n2 = graph.findNode(order->tempDeliveryId);
        if (n1 == nullptr) throw NodeDoesNotExistException(order->tempPickUpId, __func__, false);
        if (n2 == nullptr) throw NodeDoesNotExistException(order->tempDeliveryId, __func__, false);
        order->setPickUp(n1);
        order->setDelivery(n2);
        Client* c = this->findClient(order->tempClientId);
        if (c == nullptr) throw ItemNotFoundException<int>("client", order->tempClientId, __func__);
        order->setClient(c);
        c->incrementOrders();
    }
}

void Company::makeRoutes(Graph &graph) {
    std::vector<Node*> pickUps;
    for (Order* order : currentOrders) {
        order->getPickUp()->setPair(order->getDelivery());
        order->getDelivery()->setPair(order->getPickUp());
        pickUps.push_back(order->getPickUp());
    }
    std::vector<std::vector<Node*>> clusters;
    int noCluster = std::min(crew.size(), std::min(currentOrders.size(), fleet.size()));
    clusters = graph.clustering(pickUps, noCluster);
    for (unsigned int i = 0; i < noCluster; i++) {
        std::vector<Node*> nodesOrdered = graph.getRoute(fleet.at(i)->getRange(), clusters.at(i), garage);
        std::vector<int> path = graph.multiGetPath(nodesOrdered);
        crew.at(i)->setRoute(path);
    }
}
