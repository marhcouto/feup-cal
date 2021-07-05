//
// Created by marhc on 10/05/2021.
//

#ifndef ENTREGA2_FILEHANDLING_H
#define ENTREGA2_FILEHANDLING_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <time.h>
#include "../E-Stafetas/Order.h"
#include "../Algorithms/Graph.h"
#include "../Algorithms/NodesInfo.h"

class UnexpectedEndOfFileException {
    std::string message;
public:
    explicit UnexpectedEndOfFileException(std::string fileName, std::string funcName) {
        std::stringstream ss;
        ss << "Error in " << funcName << ": file " << fileName << " ended unexpectedly";
        message = ss.str();
    }
    const std::string& getMessage() const {
        return message;
    }
};

class FailedToOpenFileException {
    std::string message;
public:
    explicit FailedToOpenFileException(std::string fileName, std::string funcName) {
        std::stringstream ss;
        ss << "Error in " << funcName << ": unable to open file " << fileName;
        message = ss.str();
    }
    const std::string& getMessage() const {
        return message;
    }
};


class FileReader {
public:
    static void readFileToGraph(Graph& graph, std::string edgesFile, std::string nodesFile);
    static void readTagsFile(Graph& graph, std::string tagsFile);
    static void writeGraphToFile(const Graph& graph, std::string edgesFile, std::string nodesFile); // Only used in development
    static void randomTagMaker(const Graph& graph, std::string tagsFile); // Only used in development

    template <class T>
    static void readFile(std::vector<T*>& v, std::string file);

    template <class T>
    static void writeFile(const std::vector<T*>& v, std::string file);
};

template <class T>
void FileReader::readFile(std::vector<T*>& v,std::string file) {
    ifstream f;
    f.open("../../DatabaseFiles/" + file, std::ifstream::in);
    if (f.fail()) throw FailedToOpenFileException(file, __func__);

    while(!f.eof()) {
        T* t = new T();
        f >> *t;
        v.push_back(t);
    }
    v.erase(v.end() - 1);
    f.close();
}

template <class T>
void FileReader::writeFile(const std::vector<T*> &v, std::string file) {
    ofstream f;
    f.open("../../DatabaseFiles/" + file, std::ofstream::in | std::ofstream::trunc);
    if (f.fail()) throw FailedToOpenFileException(file, __func__);

    for (T* t : v)
        f << *t;
    f.close();
}


#endif //ENTREGA2_FILEHANDLING_H
