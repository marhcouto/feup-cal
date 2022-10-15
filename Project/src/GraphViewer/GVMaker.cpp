//
// Created by marhc on 21/05/2021.
//

#include "GVMaker.h"



GVMaker::GVMaker(Graph graph) {

    int counterID = 0;

    gv.setCenter(sf::Vector2f(1000, 1000));

    for (Node* node : graph.getNodeSet())
        node->setVisited(false);

    for (Node* node : graph.getNodeSet()) {
        int id1, id2;
        id1 = node->getId();
        if (!node->getVisited()) {
            GraphViewer::Node& node1 = gv.addNode(node->getId(),sf::Vector2f((long) node->getInfo().getLatitude(), (long) node->getInfo().getLongitude()));
            if (node->getInfo().getType() == GARAGE) {
                node1.setColor(GraphViewer::BLUE);
                node1.setLabel("G");
                node1.setSize(50.0);
            } else if (node->getInfo().getType() == RECHARGE) {
                node1.setColor(GraphViewer::GREEN);
                node1.setLabel("R");
                node1.setSize(30.0);
            } else if (node->getInfo().getType() == PICKUP) {
                node1.setColor(GraphViewer::YELLOW);
                node1.setLabel("PUP");
                node1.setSize(30.0);
            } else if (node->getInfo().getType() == DELIVERY) {
                node1.setColor(GraphViewer::ORANGE);
                node1.setLabel("D");
                node1.setSize(30.0);
            }
            node->setVisited(true);
        }
        for (Edge* edge : node->getAdj()) {
            id2 = edge->getDest()->getId();
            if (!edge->getDest()->getVisited()) {
                GraphViewer::Node &node2 = gv.addNode(edge->getDest()->getId(),sf::Vector2f(edge->getDest()->getInfo().getLatitude(),
                                                                                            edge->getDest()->getInfo().getLongitude()));
                edge->getDest()->setVisited(true);
            }
            gv.addEdge(counterID, gv.getNode(id1), gv.getNode(id2), GraphViewer::Edge::EdgeType::DIRECTED);
            counterID++;
        }
    }

    for (Node* node : graph.getNodeSet())
        node->setVisited(false);
}

void GVMaker::paintRoutes(std::vector<int> route) {
    for (int i : route) {
        GraphViewer::Node& n = gv.getNode(i);
        n.setColor(GraphViewer::GRAY);
        n.setSize(20.0);
    }
}

void GVMaker::display() {
    gv.createWindow();
    gv.join();
}