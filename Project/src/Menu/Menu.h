//
// Created by marhc on 24/05/2021.
//

#ifndef ENTREGA2_MENU_H
#define ENTREGA2_MENU_H

#include <string>
#include <iostream>
#include <utility>

#include "../GraphViewer/GVMaker.h"
#include "../E-Stafetas/Company.h"
#include "../Algorithms/Graph.h"
#include "../FileHandling/FileHandling.h"

class Menu {
    Company company;
    Graph graph;
public:
    Menu();
    Menu(Graph graph, Company company);
    void companyMenu();
    void startingMenu();
    void graphMenu();
    void orderMenu();
    void loadingMenu();
};


#endif //ENTREGA2_MENU_H
