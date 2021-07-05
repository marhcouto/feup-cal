//
// Created by marhc on 13/05/2021.
//

#ifndef ENTREGA2_GVMAKER_H
#define ENTREGA2_GVMAKER_H

#include <sstream>
#include "graphviewer.h"
#include "../Algorithms/Graph.h"

class GVMaker {
    GraphViewer gv;
public:
    explicit GVMaker(Graph graph);

    void paintRoutes(std::vector<int> route);
    void display();
};



#endif //ENTREGA2_GVMAKER_H
