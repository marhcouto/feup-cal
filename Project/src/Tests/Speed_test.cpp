//
// Created by marhc on 16/05/2021.
//

#include <chrono>
#include "gtest/gtest.h"
#include "../FileHandling/FileHandling.h"

TEST(Speed_test, Shortest_Path_test) {
    Graph graph1;
    Graph graph2;

    ASSERT_NO_THROW(FileReader::readFileToGraph(graph1, "edges.txt", "nodes.txt"));
    ASSERT_NO_THROW(FileReader::readFileToGraph(graph2, "edges.txt", "nodes.txt"));

    graph1.assignIDM();
    graph2.assignIDM();

    auto time1 = std::chrono::high_resolution_clock::now();
    graph1.dijkstraMulti();
    auto time2 = std::chrono::high_resolution_clock::now();
    graph2.floydWarshallShortestPath();
    auto time3 = std::chrono::high_resolution_clock::now();

    std::cout << "Dijkstra's:" << std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << std::endl;
    std::cout << "FloydWarshall's:" << std::chrono::duration_cast<std::chrono::microseconds>(time3 - time2).count() << std::endl;

}

TEST(Speed_test, Connectivity_test) {
    Graph graph1;

    ASSERT_NO_THROW(FileReader::readFileToGraph(graph1, "porto_full_edges.txt", "porto_full_nodes.txt"));

    graph1.assignIDM();
    auto time1 = std::chrono::high_resolution_clock::now();
    graph1.tarjan();
    ASSERT_NO_THROW(graph1.eliminateInaccessible(1));
    auto time2 = std::chrono::high_resolution_clock::now();

    std::cout << "Tarjan:" << std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() << std::endl;
}

TEST(Speed_test, Bidirectional) {
    Graph myGraph;

    EXPECT_NO_THROW(FileReader::readFileToGraph(myGraph, "porto_connected_edges.txt", "porto_connected_nodes.txt"));

    auto time1 = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < myGraph.getNodeSet().size(); j++) {
            if (j == i) continue;
            int n1 = myGraph.getNodeSet()[i]->getId();
            int n2 = myGraph.getNodeSet()[j]->getId();
            std::cout << n1 << "->" << n2 << std::endl;
            EXPECT_NO_THROW(myGraph.bidirectionalDijkstra(n1, n2));
        }
    }


    auto time2 = std::chrono::high_resolution_clock::now();
    std::cout << "Bidirectional:" << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "ms" << std::endl;
}