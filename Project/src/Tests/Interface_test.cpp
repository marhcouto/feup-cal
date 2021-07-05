//
// Created by marhc on 19/05/2021.
//

#include "gtest/gtest.h"
#include "../Algorithms/Graph.h"
#include "../E-Stafetas/Company.h"
#include "../FileHandling/FileHandling.h"


TEST(Interface, resolveOrders) {

    std::vector<Order*> orders;
    std::vector<Client*> clients;
    std::vector<Driver*> drivers;
    std::vector<Vehicle*> vehicles;
    Graph graph;

    graph.addNode(NodeInfo(10, 10), 1);
    graph.addNode(NodeInfo(10, 10), 2);
    graph.addNode(NodeInfo(10, 10), 3);
    graph.addNode(NodeInfo(10, 10), 4);
    graph.addNode(NodeInfo(10, 10), 5);
    graph.addNode(NodeInfo(10, 10), 6);

    clients.push_back(new Client("Burp"));
    clients.push_back(new Client("Berp"));

    Order* o1 = new Order();
    o1->tempClientId = 1;
    o1->tempDeliveryId = 1;
    o1->tempPickUpId = 2;

    Order* o2 = new Order();
    o2->tempClientId = 2;
    o2->tempDeliveryId = 3;
    o2->tempPickUpId = 4;

    Order* o3 = new Order();
    o3->tempPickUpId = 4;
    o3->tempDeliveryId = 5;
    o3->tempClientId = 2;

    orders.push_back(o1);
    orders.push_back(o2);
    orders.push_back(o3);

    Company company(drivers, clients, orders, vehicles, nullptr);

    ASSERT_NO_THROW(company.assignOrdersParameters(graph));

    for (const auto& order : company.getOrders()) {
        order->print();
    }
}

TEST(Interface, GetRoute) {
    Graph graph;

    Node* garage;

    try {
        FileReader::readFileToGraph(graph, "porto_connected_edges.txt", "porto_connected_nodes.txt");
        FileReader::readTagsFile(graph, "recharge_points_porto.txt");
    } catch (FailedToOpenFileException& e) {
        std::cout << e.getMessage() << std::endl;
    } catch (NodeDoesNotExistException& e) {
        std::cout << e.getMessage() << std::endl;
    }
    garage = graph.findNode(3);
    garage->getInfo().setType(GARAGE);

    std::vector<Driver*> crew;
    std::vector<Order*> orders;
    std::vector<Vehicle*> fleet;
    std::vector<Client*> clients;

    try {
        FileReader::readFile<Client>(clients, "Clients.txt");
        FileReader::readFile<Vehicle>(fleet, "Vehicles.txt");
        FileReader::readFile<Driver>(crew, "Drivers.txt");
        FileReader::readFile<Order>(orders, "Orders.txt");
    } catch(FailedToOpenFileException e) {
        std::cout << e.getMessage() << std::endl;
    }
    Company company(crew, clients, orders, fleet, garage);

    try {
        company.assignOrdersParameters(graph);
    } catch (ItemNotFoundException<int>& e) {
        std::cout << e.getMessage() << std::endl;
    } catch (NodeDoesNotExistException& e) {
        std::cout << e.getMessage() << std::endl;
    }

    Node* n1 = graph.findNode(41);
    Node* n2 = graph.findNode(53);
    Node* n3 = graph.findNode(102);
    Node* n4 = graph.findNode(158);
    Node* n5 = graph.findNode(297);
    Node* n6 = graph.findNode(294);
    Client* c = company.findClient(2);

    company.addOrder(new Order(n1, n2, c, "Yeah"));
    company.addOrder(new Order(n3, n4, c, "Yeah"));
    company.addOrder(new Order(n5, n6, c, "Yeah"));

    company.makeRoutes(graph);

    for (const auto& d : company.getCrew())
        d->printRoute();
}