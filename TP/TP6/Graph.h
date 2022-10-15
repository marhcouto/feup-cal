/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "MutablePriorityQueue.h"


template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
    T info;						// content of the vertex
    std::vector<Edge<T> > adj;		// outgoing edges

    double dist = 0;
    Vertex<T> *path = NULL;
    int queueIndex = 0; 		// required by MutablePriorityQueue

    bool visited = false;		// auxiliary field
    bool processing = false;	// auxiliary field

    void addEdge(Vertex<T> *dest, double w);

public:
    Vertex(T in);
    T getInfo() const;
    double getDist() const;
    Vertex *getPath() const;

    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    friend class Graph<T>;
    friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
    Vertex<T> * dest;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);
    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set
    std::vector<std::vector<double>> floydWarshallMatrix;
    std::vector<std::vector<int>> next;

public:
    Vertex<T> *findVertex(const T &in) const;
    Vertex<T> *findVertexByIndex(int &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    // Fp06 - single source
    void unweightedShortestPath(const T &s);    //TODO...
    void dijkstraShortestPath(const T &s);      //TODO...
    void bellmanFordShortestPath(const T &s);   //TODO...
    std::vector<T> getPath(const T &origin, const T &dest) const;   //TODO...

    // Fp06 - all pairs
    void floydWarshallShortestPath();   //TODO...
    std::vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;   //TODO...

};

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

/*
* Auxiliary function to find a vertex with a given index.
*/
template <class T>
Vertex<T> * Graph<T>::findVertexByIndex(int &in) const {
    for (auto v : vertexSet)
        if (v->queueIndex == in)
            return v;
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if ( findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w);
    return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) { //Performs a breadth-first search and sets the distances to the origin along the way

    for (Vertex<T>* vertex : this->vertexSet) {
        vertex->dist = INF;
        vertex->path = NULL;
    }

    std::queue<Vertex<T>*> vertexQueue;
    this->findVertex(orig)->dist = 0;
    vertexQueue.push(this->findVertex(orig));

    while (!vertexQueue.empty()) {
        Vertex<T>* v = vertexQueue.front();
        vertexQueue.pop();
        for (Edge<T> edge : v->adj) {
            if (edge.dest->dist == INF) { //If it was visited, then surely the distance is smaller, because edges weight is always one
                edge.dest->dist = v->dist + 1;
                edge.dest->path = v;
                vertexQueue.push(edge.dest);
            }
        }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) { //Same as above but edges' weight is not always one
    for (Vertex<T>* vertex : vertexSet) {
        vertex->dist = INF;
        vertex->path = NULL;
        vertex->queueIndex = 0; //For the order in the priority queue
        vertex->visited = false;
    }

    MutablePriorityQueue<Vertex<T>> vertexQueue; //Allows decreasekey
    findVertex(origin)->dist = 0;
    vertexQueue.insert(findVertex(origin));

    while(!vertexQueue.empty()) {
        Vertex<T>* v = vertexQueue.extractMin();
        v->visited = true;
        for (Edge<T> edge : v->adj) {
            if (edge.dest->dist > v->dist + edge.weight && !edge.dest->visited) {
                double oldDist = edge.dest->dist;
                edge.dest->dist = v->dist + edge.weight;
                edge.dest->path = v;
                if (oldDist == INF) //Is not in the queue
                    vertexQueue.insert(edge.dest);
                else //Increase its priority so that the algorithm maintains an optimal time
                    vertexQueue.decreaseKey(edge.dest);
            }
        }
    }
}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) { //Negative weight edges
    for (Vertex<T>* vertex : vertexSet) {
        vertex->dist = INF;
        vertex->path = NULL;
    }
    std::queue<Vertex<T>*> vertexQueue;
    findVertex(orig)->dist = 0;
    vertexQueue.push(findVertex(orig));

    for (unsigned int i = 0; i < this->vertexSet.size(); i++) { //Repeat |V| times
        for (unsigned int i = 0; i < vertexSet.size(); i++)
            vertexSet[i]->visited = false;
        while (!vertexQueue.empty()) { //Go through all edges
            Vertex<T>* v = vertexQueue.front();
            vertexQueue.pop();
            for (Edge<T> edge : v->adj) {
                if (edge.dest->dist > v->dist + edge.weight) {
                    edge.dest->dist = v->dist + edge.weight;
                    edge.dest->path = v;
                }
                if (!edge.dest->visited)
                    vertexQueue.push(edge.dest);
                edge.dest->visited = true;
            }
        }
    }
    //Missing check for negative cycles
}


template<class T>
std::vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
    std::vector<T> res;

    Vertex<T>* v = this->findVertex(dest);
    if (v == nullptr || v->dist == INF) return res;

    while(true) {
        res.push_back(v->info);
        if (v->info == origin) break;
        v = v->path;
    }

    std::reverse(res.begin(), res.end());

    return res;
}



/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() { //Makes matrix with all paths

    for (unsigned int i = 0; i < vertexSet.size(); i++)
        vertexSet[i]->queueIndex = i;

    for (unsigned int i = 0; i < vertexSet.size(); i++) {
        std::vector<double> temp1;
        std::vector<int> temp2;
        for (unsigned int j = 0; j < vertexSet.size(); j++) {
            temp1.push_back(INF);
            temp2.push_back(0);
        }
        floydWarshallMatrix.push_back(temp1); //Weights
        next.push_back(temp2); //Paths
    }

    for (Vertex<T>* vertex : vertexSet) {
        for (Edge<T> edge : vertex->adj) { //Initial fill in
            floydWarshallMatrix[vertex->queueIndex][edge.dest->queueIndex] = edge.weight;
            next[vertex->queueIndex][edge.dest->queueIndex] = edge.dest->queueIndex;
        }
    }

    for (unsigned int i = 0; i < vertexSet.size(); i++) {
        for (unsigned int j = 0; j < vertexSet.size(); j++) {
            for (unsigned int k = 0; k < vertexSet.size(); k++) {
                if (floydWarshallMatrix[j][k] > floydWarshallMatrix[j][i] + floydWarshallMatrix[i][k]) { //Checks if going through i makes the path shorter
                    floydWarshallMatrix[j][k] = floydWarshallMatrix[j][i] + floydWarshallMatrix[i][k];
                    next[j][k] = next[j][i];
                }
            }
        }
    }
}

template<class T>
std::vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{ //Calculate the paths
    std::vector<T> res;
    Vertex<T>* originV = this->findVertex(orig);
    Vertex<T>* destV = this->findVertex(dest);
    if (originV == nullptr || destV == nullptr) return res;

    res.push_back(orig);
    int nextI = originV->queueIndex;

    while (destV->queueIndex != nextI) {
        nextI = next[nextI][destV->queueIndex];
        res.push_back(findVertexByIndex(nextI)->info);
    }

    return res;
}


#endif /* GRAPH_H_ */
