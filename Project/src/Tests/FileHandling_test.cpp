//
// Created by marhc on 13/05/2021.
//

#include "gtest/gtest.h"
#include "../FileHandling/FileHandling.h"
#include "../E-Stafetas/Vehicle.h"
#include "../E-Stafetas/Driver.h"
#include "../E-Stafetas/Client.h"
#include "../E-Stafetas/Order.h"

TEST(FileHandling, readingGraph) {
    Graph graph;

    EXPECT_NO_THROW(FileReader::readFileToGraph(graph, "edges.txt", "nodes.txt"));
    EXPECT_EQ(graph.getNodeSet().size(), 289);
    EXPECT_EQ(graph.findNode(0)->getAdj().size(), 2);
    EXPECT_EQ(graph.findNode(1)->getAdj().size(), 2);
    EXPECT_EQ(graph.findNode(125)->getAdj().size(), 2);
    EXPECT_EQ(graph.findNode(286)->getAdj().size(), 1);
    EXPECT_EQ(graph.findNode(288)->getAdj().size(), 0);
    for (Node* n : graph.getNodeSet()) {
        for (Edge* e : n->getAdj()) {
            EXPECT_EQ(e->getWeight(), 37);
        }
    }
}

TEST(FileHandling, readingVehicles) {
    std::vector<Vehicle*> v;
    EXPECT_NO_THROW(FileReader::readFile<Vehicle>(v, "Vehicles.txt"));

    EXPECT_EQ(v.size(), 5);
    for (const auto& vehicle : v) {
        EXPECT_EQ(vehicle->getMaxRange(), 30000);
        EXPECT_EQ(vehicle->getRange(), 30000);
        // std::cout << "License Plate:" << vehicle->getLicensePlate() << std::endl;
    }
}

TEST(FileHandling, readingDrivers) {
    std::vector<Driver*> v;
    EXPECT_NO_THROW(FileReader::readFile<Driver>(v, "Drivers.txt"));

    EXPECT_EQ(v.size(), 5);
    int id = 1;
    for (const auto& driver : v) {
        EXPECT_EQ(id, driver->getId());
        // std::cout << "Name:" << driver->getName() << " Age:" << driver->getAge() << " Salary:" << driver->getSalary() << " Years:" << driver->getYearsExperience() << std::endl;
        id++;
    }
}

TEST(FileHandling, readingClients) {
    std::vector<Client*> v;
    EXPECT_NO_THROW(FileReader::readFile<Client>(v, "Clients.txt"));

    EXPECT_EQ(v.size(), 8);
    int id = 1;
    for (const auto& client : v) {
        EXPECT_EQ(id, client->getId());
        EXPECT_EQ(0, client->getNoOrders());
        id++;
        // std::cout << client->getName() << " " << client->getId() << " " << client->getNoOrders() << std::endl;
    }
}

TEST(FileHandling, readingOrders) {
    std::vector<Order*> v;
    EXPECT_NO_THROW(FileReader::readFile<Order>(v, "Orders.txt"));

    EXPECT_EQ(v.size(), 11);

    int id = 1;
    for (const auto& order : v) {
        EXPECT_EQ(id, order->getOrderId());
        EXPECT_TRUE(order->getDate().size() > 0);
        id++;
        std::cout << "Order:" << order->getOrderId() << " Date:" << order->getDate() << " PickUp:" << order->tempPickUpId << " Delivery:" <<
        order->tempDeliveryId << " Client:" << order->tempClientId << std::endl;
    }
}
