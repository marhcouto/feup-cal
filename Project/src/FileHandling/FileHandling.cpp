//
// Created by marhc on 16/05/2021.
//

#include "FileHandling.h"

void FileReader::readFileToGraph(Graph& graph, std::string edgesFile, std::string nodesFile) {

    std::cout << "Starting reading from file\n";

    ifstream f;

    // Nodes
    f.open("../../GraphFiles/" + nodesFile, std::ifstream::in);
    if (f.fail()) throw FailedToOpenFileException(edgesFile, __func__);

    while (!f.eof()) {
        double latitude, longitude;
        int nodeID;
        std::string line;
        char uselessChar1, uselessChar2, uselessChar3, uselessChar4;
        f >> uselessChar1 >> nodeID  >> uselessChar2 >> latitude >> uselessChar3 >> longitude >> uselessChar4;
        graph.addNode(NodeInfo(latitude, longitude), nodeID);
    }
    f.close();

    // Edges
    f.open("../../GraphFiles/" + edgesFile, std::ifstream::in);
    if (f.fail()) throw FailedToOpenFileException(edgesFile, __func__);

    while (!f.eof()) {
        int nodeID1, nodeID2;
        char uselessChar;
        f >> uselessChar >> nodeID1 >> uselessChar >> nodeID2 >> uselessChar;
        Node* node1 = graph.findNode(nodeID1);
        Node* node2 = graph.findNode(nodeID2);
        if (node1 == nullptr) throw NodeDoesNotExistException(nodeID1, __func__, false);
        if (node2 == nullptr) throw NodeDoesNotExistException(nodeID2, __func__, false);
        graph.addEdge(node1, node2, NodeInfo::getDistance(node1->getInfo().getLatitude(), node1->getInfo().getLongitude(),
                                                          node2->getInfo().getLatitude(), node2->getInfo().getLongitude()));
    }
    f.close();

    std::cout << "Ended reading from file\n";
}

void FileReader::readTagsFile(Graph &graph, std::string tagsFile) {
    fstream f;

    std::cout << "Started reading tags\n";

    f.open("../../GraphFiles/" + tagsFile, std::istream::in);
    if (f.fail()) throw FailedToOpenFileException(tagsFile, __func__);

    int i = 0;
    while (!f.eof()) {
        int id;
        f >> id;
        Node* n = graph.findNode(id);
        if (n == nullptr) throw NodeDoesNotExistException(id, __func__, false);
        n->getInfo().setType(RECHARGE);
    }
    f.close();

    std::cout << "Finished reading tags\n";
}

void FileReader::writeGraphToFile(const Graph &graph, std::string edgesFile, std::string nodesFile) {

    fstream f1, f2;
    f1 << setprecision(10);
    f2 << setprecision(10);
    f1.open("../../GraphFiles/" + nodesFile, std::ostream::out | std::ostream::trunc);
    if (f1.fail()) throw FailedToOpenFileException(nodesFile, __func__);

    f2.open("../../GraphFiles/" + edgesFile, std::ostream::out | std::ostream::trunc);
    if (f2.fail()) throw FailedToOpenFileException(edgesFile, __func__);

    for (Node* node : graph.getNodeSet()) {
        f1 << "(" << node->getId() << "," << node->getInfo().getLatitude() << "," << node->getInfo().getLongitude() << ")\n";
        for (Edge* edge : node->getAdj())
            f2 << "(" << node->getId() << "," << edge->getDest()->getId() << ")\n";
    }

    f1.close();
    f2.close();
}

void FileReader::randomTagMaker(const Graph &graph, std::string tagsFile) {
    fstream f;

    f.open("../../GraphFiles/" + tagsFile, std::ostream::out);
    if (f.fail()) throw FailedToOpenFileException(tagsFile, __func__);

    for (Node* node : graph.getNodeSet()) {
        int randomNo = rand() % 50 + 1;
        if (randomNo == 50)
            f << node->getId() << std::endl;
    }
    f.close();
}