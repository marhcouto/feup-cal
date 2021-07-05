#include <unistd.h>
#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex2() {
    GraphViewer gv;

    gv.setCenter(sf::Vector2f(300, 300));

    gv.addNode(0, sf::Vector2f(300, 50));
    gv.addNode(1, sf::Vector2f(318, 58));
    gv.addNode(2, sf::Vector2f(325, 75));
    gv.addNode(3, sf::Vector2f(318, 93));
    gv.addNode(4, sf::Vector2f(300, 100));
    gv.addNode(5, sf::Vector2f(282, 93));
    gv.addNode(6, sf::Vector2f(275, 75));
    gv.addNode(7, sf::Vector2f(282, 58));
    gv.addNode(8, sf::Vector2f(150, 200));
    gv.addNode(9, sf::Vector2f(300, 200));
    gv.addNode(10, sf::Vector2f(450, 200));
    gv.addNode(11, sf::Vector2f(300, 400));
    gv.addNode(12, sf::Vector2f(200, 550));
    gv.addNode(13, sf::Vector2f(400, 550));


    gv.addEdge(0, gv.getNode(0), gv.getNode(1), Edge::EdgeType::DIRECTED);
    gv.addEdge(1, gv.getNode(1), gv.getNode(2), Edge::EdgeType::DIRECTED);
    gv.addEdge(2, gv.getNode(2), gv.getNode(3), Edge::EdgeType::DIRECTED);
    gv.addEdge(3, gv.getNode(3), gv.getNode(4), Edge::EdgeType::DIRECTED);
    gv.addEdge(4, gv.getNode(4), gv.getNode(5), Edge::EdgeType::DIRECTED);
    gv.addEdge(5, gv.getNode(5), gv.getNode(6), Edge::EdgeType::DIRECTED);
    gv.addEdge(6, gv.getNode(6), gv.getNode(7), Edge::EdgeType::DIRECTED);
    gv.addEdge(7, gv.getNode(7), gv.getNode(0), Edge::EdgeType::DIRECTED);
    gv.addEdge(8, gv.getNode(4), gv.getNode(9), Edge::EdgeType::DIRECTED);
    gv.addEdge(9, gv.getNode(9), gv.getNode(8), Edge::EdgeType::DIRECTED);
    gv.addEdge(10, gv.getNode(9), gv.getNode(10), Edge::EdgeType::DIRECTED);
    gv.addEdge(11, gv.getNode(9), gv.getNode(11), Edge::EdgeType::DIRECTED);
    gv.addEdge(12, gv.getNode(11), gv.getNode(12), Edge::EdgeType::DIRECTED);
    gv.addEdge(13, gv.getNode(11), gv.getNode(13), Edge::EdgeType::DIRECTED);

    gv.createWindow();

    bool alternate = true;

    while(true) {
        gv.lock();
        if (alternate) {
            gv.getNode(13).setPosition(sf::Vector2f(350, 550));
            gv.getNode(12).setPosition(sf::Vector2f(250, 550));
            alternate = false;
        } else {
            gv.getNode(13).setPosition(sf::Vector2f(400, 550));
            gv.getNode(12).setPosition(sf::Vector2f(200, 550));
            alternate = true;
        }
        gv.unlock();
        sleep(1);
    }

    gv.join();
}

