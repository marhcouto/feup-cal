//
// Created by marhc on 16/05/2021.
//

#ifndef ENTREGA2_ORDER_H
#define ENTREGA2_ORDER_H

#include <iostream>
#include "../Algorithms/Graph.h"
#include "../Algorithms/NodesInfo.h"
#include "Client.h"

class Order {
    int orderId;
    std::string date;
    Node* pickUp;
    Node* delivery;
    Client* client;
public:
    int tempClientId;
    int tempPickUpId;
    int tempDeliveryId;

    int static currentId;

    Order();
    Order(Node* n1, Node* n2, Client* c, std::string date);

    int getOrderId() const;
    Node* getPickUp() const;
    Node* getDelivery() const;
    Client* getClient() const;
    const string &getDate() const;
    void setDate(const string &date);
    void setClient(Client *client);
    void setPickUp(Node *pickUp);
    void setDelivery(Node *delivery);

    void print();

    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    friend std::istream& operator>>(std::istream& is, Order& order);
};


#endif //ENTREGA2_ORDER_H
