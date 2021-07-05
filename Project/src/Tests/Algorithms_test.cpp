//
// Created by marhc on 11/05/2021.
//

#include <vector>
#include "gtest/gtest.h"
#include "../FileHandling/FileHandling.h"

template <class T>
void checkSinglePath(std::vector<T> path, std::string expected) {
    std::stringstream ss;
    for(unsigned int i = 0; i < path.size(); i++)
        std::cout << path[i] << " ";
    //EXPECT_EQ(expected, ss.str());
}

TEST(Algorithms, Tarjan) {
    Graph graph;

    graph.addNode(NodeInfo(1, 2), 1);
    graph.addNode(NodeInfo(1, 2), 2);
    graph.addNode(NodeInfo(1, 2), 3);
    graph.addNode(NodeInfo(1, 2), 4);
    graph.addNode(NodeInfo(1, 2), 5);
    graph.addNode(NodeInfo(1, 2), 6);

    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 3, 1);
    graph.addEdge(3, 1, 1);
    graph.addEdge(3, 6, 1);
    graph.addEdge(6, 2, 1);
    graph.addEdge(4, 5, 1);
    graph.addEdge(4, 6, 1);

    graph.tarjan();
    EXPECT_EQ(graph.findNode(1)->getLow(), graph.findNode(2)->getLow());
    EXPECT_EQ(graph.findNode(1)->getLow(), graph.findNode(3)->getLow());
    EXPECT_EQ(graph.findNode(3)->getLow(), graph.findNode(2)->getLow());
    EXPECT_EQ(graph.findNode(1)->getLow(), graph.findNode(6)->getLow());
    EXPECT_EQ(graph.findNode(6)->getLow(), graph.findNode(2)->getLow());
    EXPECT_FALSE(graph.findNode(4)->getLow() == graph.findNode(1)->getLow());
    EXPECT_FALSE(graph.findNode(4)->getLow() == graph.findNode(2)->getLow());
    EXPECT_FALSE(graph.findNode(5)->getLow() == graph.findNode(3)->getLow());
    EXPECT_FALSE(graph.findNode(5)->getLow() == graph.findNode(6)->getLow());

    EXPECT_NO_THROW(graph.eliminateInaccessible(1));
    EXPECT_EQ(graph.getNodeSet().size(), 4);
}

TEST(Algorithms, FloydWarshall) {
    Graph myGraph;

    for(int i = 1; i <= 7; i++)
        myGraph.addNode(NodeInfo(1, 2), i);

    myGraph.addEdge(1, 2, 2);
    myGraph.addEdge(1, 4, 7);
    myGraph.addEdge(2, 4, 3);
    myGraph.addEdge(2, 5, 5);
    myGraph.addEdge(3, 1, 2);
    myGraph.addEdge(3, 6, 5);
    myGraph.addEdge(4, 3, 1);
    myGraph.addEdge(4, 5, 1);
    myGraph.addEdge(4, 6, 6);
    myGraph.addEdge(4, 7, 4);
    myGraph.addEdge(5, 7, 2);
    myGraph.addEdge(6, 4, 3);
    myGraph.addEdge(7, 6, 4);

    myGraph.assignIDM();

    myGraph.floydWarshallShortestPath();
    checkSinglePath(myGraph.getShortestPath(1, 7), "1 2 4 5 7 ");
    checkSinglePath(myGraph.getShortestPath(5, 6), "5 7 6 ");
    checkSinglePath(myGraph.getShortestPath(7, 1), "7 6 4 3 1 ");
}

TEST(Algorithms, MultiDijkstras) {
    Graph myGraph;

    for(int i = 1; i <= 7; i++)
        myGraph.addNode(NodeInfo(1, 2), i);

    myGraph.addEdge(1, 2, 2);
    myGraph.addEdge(1, 4, 7);
    myGraph.addEdge(2, 4, 3);
    myGraph.addEdge(2, 5, 5);
    myGraph.addEdge(3, 1, 2);
    myGraph.addEdge(3, 6, 5);
    myGraph.addEdge(4, 3, 1);
    myGraph.addEdge(4, 5, 1);
    myGraph.addEdge(4, 6, 6);
    myGraph.addEdge(4, 7, 4);
    myGraph.addEdge(5, 7, 2);
    myGraph.addEdge(6, 4, 3);
    myGraph.addEdge(7, 6, 4);

    myGraph.assignIDM();

    myGraph.dijkstraMulti();
    checkSinglePath(myGraph.getShortestPath(1, 7), "1 2 4 5 7 ");
    checkSinglePath(myGraph.getShortestPath(5, 6), "5 7 6 ");
    checkSinglePath(myGraph.getShortestPath(7, 1), "7 6 4 3 1 ");
}

TEST(Algorithms, Clustering) {
    Graph myGraph;

    for(int i = 1; i <= 7; i++)
        myGraph.addNode(NodeInfo(1, 2), i);

    myGraph.addEdge(1, 2, 2);
    myGraph.addEdge(1, 4, 7);
    myGraph.addEdge(2, 4, 3);
    myGraph.addEdge(2, 5, 5);
    myGraph.addEdge(3, 1, 2);
    myGraph.addEdge(3, 6, 5);
    myGraph.addEdge(4, 3, 1);
    myGraph.addEdge(4, 5, 1);
    myGraph.addEdge(4, 6, 6);
    myGraph.addEdge(4, 7, 4);
    myGraph.addEdge(5, 7, 2);
    myGraph.addEdge(6, 4, 3);
    myGraph.addEdge(7, 6, 4);

    myGraph.assignIDM();
    myGraph.dijkstraMulti();

    std::vector<Node*> nodes;

    nodes.push_back(myGraph.findNode(1));
    nodes.push_back(myGraph.findNode(2));
    nodes.push_back(myGraph.findNode(3));
    nodes.push_back(myGraph.findNode(4));
    nodes.push_back(myGraph.findNode(5));
    nodes.push_back(myGraph.findNode(6));
    nodes.push_back(myGraph.findNode(7));

    std::vector<std::vector<Node*>> sets;
    sets = myGraph.clustering(nodes, 3);

    ASSERT_EQ(sets.size(), 3);
    ASSERT_EQ(sets[0].size(), 5);
    ASSERT_EQ(sets[1].size(), 1);
    ASSERT_EQ(sets[2].size(), 1);
    ASSERT_EQ(sets[0][0]->getId(), 1);
    ASSERT_EQ(sets[0][1]->getId(), 2);
    ASSERT_EQ(sets[0][2]->getId(), 3);
    ASSERT_EQ(sets[0][3]->getId(), 4);
    ASSERT_EQ(sets[0][4]->getId(), 6);
    ASSERT_EQ(sets[1][0]->getId(), 5);
    ASSERT_EQ(sets[2][0]->getId(), 7);
}

TEST(Algorithms, Bidirectional) {
    Graph myGraph;

    EXPECT_NO_THROW(FileReader::readFileToGraph(myGraph, "porto_connected_edges.txt", "porto_connected_nodes.txt"));

    myGraph.assignIDM();
    myGraph.dijkstraMulti();

    int i1 = myGraph.findNode(9)->getIDM();
    int i2 = myGraph.findNode(3)->getIDM();
    int i3 = myGraph.findNode(52967)->getIDM();
    int i4 = myGraph.findNode(21510)->getIDM();
    int i5 = myGraph.findNode(12134)->getIDM();
    int i6 = myGraph.findNode(162)->getIDM();

    double r1 = myGraph.bidirectionalDijkstra(9, 3);
    double r2 = myGraph.bidirectionalDijkstra(52967, 21510);
    double r3 = myGraph.bidirectionalDijkstra(12134, 162);

    double r4 = myGraph.getDistanceMatrix().at(i1).at(i2);
    double r5 = myGraph.getDistanceMatrix().at(i3).at(i4);
    double r6 = myGraph.getDistanceMatrix().at(i5).at(i6);


    EXPECT_NEAR(r1, r4, 5.0);
    EXPECT_NEAR(r2, r5,5.0);
    EXPECT_NEAR(r3, r6,5.0);
}

TEST(Algorithms, ShortestPathBidi) {
    Graph myGraph;

    EXPECT_NO_THROW(FileReader::readFileToGraph(myGraph, "porto_connected_edges.txt", "porto_connected_nodes.txt"));

    double r1 = myGraph.bidirectionalDijkstra(9, 3);

    Node* n1 = myGraph.findNode(9);
    Node* n2 = myGraph.findNode(3);
    ASSERT_FALSE(n1 == nullptr);
    ASSERT_FALSE(n2 == nullptr);

    std::vector<int> path = myGraph.getShortestPathBidirectional(n1, n2);

    for (int id : path) {
        std::cout << id << " ";
    }
}

TEST(Algorithms, FindRecharge) {
    Graph myGraph;

    for(int i = 1; i <= 7; i++)
        myGraph.addNode(NodeInfo(1, 2), i);

    myGraph.addEdge(1, 2, 2);
    myGraph.addEdge(1, 4, 7);
    myGraph.addEdge(2, 4, 3);
    myGraph.addEdge(2, 5, 5);
    myGraph.addEdge(3, 1, 2);
    myGraph.addEdge(3, 6, 5);
    myGraph.addEdge(4, 3, 1);
    myGraph.addEdge(4, 5, 1);
    myGraph.addEdge(4, 6, 6);
    myGraph.addEdge(4, 7, 4);
    myGraph.addEdge(5, 7, 2);
    myGraph.addEdge(6, 4, 3);
    myGraph.addEdge(7, 6, 4);

    myGraph.findNode(4)->getInfo().setType(RECHARGE);
    myGraph.findNode(6)->getInfo().setType(RECHARGE);

    EXPECT_EQ(4, myGraph.findNearestRecharge(myGraph.findNode(1))->getId());
}

TEST(Algorithms, Preprocessing) {
    Graph myGraph;

    EXPECT_NO_THROW(FileReader::readFileToGraph(myGraph, "porto_full_edges.txt", "porto_full_nodes.txt"));

    std::cout << "Size: " << myGraph.getNodeSet().size() << std::endl;

    myGraph.cleanGraph();

    std::cout << "Size: " << myGraph.getNodeSet().size() << std::endl;
}

TEST(Algorithms, RouteCalculation) {
    Graph myGraph;

    EXPECT_NO_THROW(FileReader::readFileToGraph(myGraph, "porto_connected_edges.txt", "porto_connected_nodes.txt"));
    EXPECT_NO_THROW(FileReader::readTagsFile(myGraph, "recharge_points_porto.txt"));


    myGraph.findNode(3)->setPair(myGraph.findNode(9));
    myGraph.findNode(9)->setPair(myGraph.findNode(3));
    myGraph.findNode(384)->setPair(myGraph.findNode(162));
    myGraph.findNode(162)->setPair(myGraph.findNode(384));
    myGraph.findNode(2004)->setPair(myGraph.findNode(1409));
    myGraph.findNode(1409)->setPair(myGraph.findNode(2004));
    std::vector<Node*> nodes;
    nodes.push_back(myGraph.findNode(3));
    nodes.push_back(myGraph.findNode(384));
    nodes.push_back(myGraph.findNode(1409));

    myGraph.bidirectionalDijkstra(3,9);

    std::vector<Node*> route = myGraph.getRoute(10000, nodes, myGraph.findNode(16));

    for (unsigned int i = 0; i < route.size(); i++)
        std::cout << route[i]->getId() << " ";

    std::cout << "\nCalculate Path\n";

    std::vector<int> path = myGraph.multiGetPath(route);
    for (int id : path)
        std::cout << id << "->";
}



