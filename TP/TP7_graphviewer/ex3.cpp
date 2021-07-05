#include <fstream>
#include <iostream>
#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex3() {

    GraphViewer gv;
    gv.setCenter(sf::Vector2f(300, 300));

    std::ifstream f;
    f.open("../TP7_graphviewer/resources/map1/nodes.txt");

    int n;
    f >> n;
    std::cout << "n:" << n << std::endl;
    for (unsigned int i = 0; i < n; i++) {
        int id, x, y;
        f >> id >> x >> y;
        std::cout << id << " " << x << " " << y << std::endl;
        gv.addNode(id, sf::Vector2f(x, y));
    }

    f.close();

    f.open("../TP7_graphviewer/resources/map1/edges.txt");

    f >> n;
    std::cout << "n:" << n << std::endl;
    for (unsigned int i = 0; i < n; i++) {
        int id, id1, id2;
        f >> id >> id1 >> id2;
        std::cout << id << " " << id1 << " " << id2 << std::endl;
        gv.addEdge(id, gv.getNode(id1), gv.getNode(id2));
    }


    gv.createWindow();
    gv.join();

}

