#include <iostream>
#include <iomanip>
#include "Menu/Menu.h"




int main(int argc, char** argv) {

    std::cout << setprecision(10);

    Menu menu;
    menu.loadingMenu();
    menu.startingMenu();

    return 0;
}
