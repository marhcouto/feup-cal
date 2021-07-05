#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex1() {
    GraphViewer gv;

    gv.setCenter(sf::Vector2f(300, 300));

    //Creating graph
    Node &node0 = gv.addNode(0, sf::Vector2f(200, 300));
    node0.setColor(GraphViewer::BLUE);

    Node &node1 = gv.addNode(1, sf::Vector2f(400, 300));
    node1.setColor(GraphViewer::BLUE);

    Edge &edge0 = gv.addEdge(0, node0, node1, Edge::EdgeType::UNDIRECTED);
    edge0.setColor(GraphViewer::BLACK);

    gv.removeNode(1);

    Node &node2 = gv.addNode(2, sf::Vector2f(500, 300));
    node2.setColor(GraphViewer::GREEN);
    node2.setLabel("Node 2");

    Edge &edge1 = gv.addEdge(1, node0, node2, Edge::EdgeType::UNDIRECTED);
    edge1.setColor(GraphViewer::BLACK);
    edge1.setLabel("0 - 2");

    for (Edge *edge : gv.getEdges() ) {
        edge->setColor(GraphViewer::YELLOW);
    }

    gv.setBackground("../TP7_graphviewer/resources/background.png");

    gv.createWindow();
    gv.join();
}
