/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>

template <class T> class Edge;
template <class T> class Graph;

constexpr auto INF = std::numeric_limits<double>::max();

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
    T info;
    std::vector<Edge<T> *> outgoing;  // adj
    std::vector<Edge<T> *> incoming;
    Edge<T> * addEdge(Vertex<T> *dest, double c, double f);
    Vertex(T in);

    bool visited;  // for path finding
    Edge<T> *path; // for path finding

public:
    T getInfo() const;
    std::vector<Edge<T> *> getAdj() const;
    friend class Graph<T>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {
}

template <class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *dest, double c, double f) {
    Edge<T> * e = new Edge<T>(this, dest, c, f);
    this->outgoing.push_back(e);
    dest->incoming.push_back(e);
    return e;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getAdj() const {
    return this->outgoing;
}


/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
    Vertex<T> * orig;
    Vertex<T> * dest;
    double capacity;
    double flow;
    Edge(Vertex<T> *o, Vertex<T> *d, double c, double f=0);

public:
    double getFlow() const;
    Vertex<T> *getDest() const;

    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *o, Vertex<T> *d, double w, double f): orig(o), dest(d), capacity(w), flow(f){}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
Vertex<T>* Edge<T>::getDest() const {
    return dest;
}


/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;
    Vertex<T>* findVertex(const T &inf) const;
    void resetFlows();
    bool findAugmentationPath(Vertex<T> *s, Vertex<T> *t);
    void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual);
    double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);
    void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double flow);
public:
    std::vector<Vertex<T> *> getVertexSet() const;
    Vertex<T> *addVertex(const T &in);
    Edge<T> *addEdge(const T &sourc, const T &dest, double c, double f=0);
    void fordFulkerson(T source, T target);

};

template <class T>
Vertex<T> * Graph<T>::addVertex(const T &in) {
    Vertex<T> *v = findVertex(in);
    if (v != nullptr)
        return v;
    v = new Vertex<T>(in);
    vertexSet.push_back(v);
    return v;
}

template <class T>
Edge<T> * Graph<T>::addEdge(const T &sourc, const T &dest, double c, double f) {
    auto s = findVertex(sourc);
    auto d = findVertex(dest);
    if (s == nullptr || d == nullptr)
        return nullptr;
    else
        return s->addEdge(d, c, f);
}

template <class T>
Vertex<T>* Graph<T>::findVertex(const T & inf) const {
    for (auto v : vertexSet)
        if (v->info == inf)
            return v;
    return nullptr;
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}


/**
 * Finds the maximum flow in a graph using the Ford Fulkerson algorithm
 * (with the improvement of Edmonds-Karp).
 * Assumes that the graph forms a flow network from source vertex 's'
 * to sink vertex 't' (distinct vertices).
 * Receives as arguments the source and target vertices (identified by their contents).
 * The result is defined by the "flow" field of each edge.
 */
template <class T>
void Graph<T>::fordFulkerson(T source, T target) {
    this->resetFlows();

    Vertex<T>* v1 = this->findVertex(source);
    Vertex<T>* v2 = this->findVertex(target);
    if (v1 == nullptr || v2 == nullptr) return;

    while (this->findAugmentationPath(v1, v2)) { //Get augmentationPath (Edmons-Karp)
        double f = findMinResidualAlongPath(v1, v2); //Calculate maxflow
        augmentFlowAlongPath(v1, v2, f); //Set maxflow to edges
    }
}

template <class T>
void Graph<T>::augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double flow) { //Sets the edges flow to the maxflow

    Vertex<T>* v = t;

    while (v->info != s->info) {
        Edge<T>* edge = v->path;
        if (edge->dest->info == v->info) {
            edge->flow += flow;
            v = edge->orig;
        }
        else { //Represents the inverted edges of the residual graph
            edge->flow -= flow;
            v = edge->dest;
        }
    }
}

template <class T>
double Graph<T>::findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) { //Starts from the end and goes to the beginning in order to find the maxFlow

    double f = INF; //maxFlow
    Vertex<T>* v = t;

    while (v->info != s->info) {
        Edge<T>* edge = v->path;
        if (edge->dest->info == v->info) {
            f = std::min(f, edge->capacity - edge->flow);
            v = edge->orig;
        }
        else { //Represents the inverted edges of the residual graph
            f = std::min(f, edge->flow);
            v = edge->dest;
        }
    }

    return f;
}

template <class T>
bool Graph<T>::findAugmentationPath(Vertex<T> *s, Vertex<T> *t) { //Edmonds-Karp

    for (Vertex<T>* vertex : this->vertexSet)
        vertex->visited = false;

    s->visited = true;
    std::queue<Vertex<T>*> q;
    q.push(s);

    //Does a breadth-first search to find an augmentationPath
    //Difference in the presence of incoming edges too, because some edges are rejected (see testAndVisit) and those nodes are left unvisited

    while(!q.empty() && !t->visited) {
        Vertex<T>* v = q.front();
        q.pop();

        for (Edge<T>* e : v->outgoing) {
            this->testAndVisit(q, e, e->dest, e->capacity - e->flow);
        }

        for (Edge<T>* e : v->incoming) { //For the inverted edges of the residual graph
            this->testAndVisit(q, e, e->orig, e->flow);
        }
    }

    //True if it has found an augmentationPath
    return t->visited;
}

template <class T>
void Graph<T>::resetFlows() { //Sets flows to 0 in the beginning of the algorithm
    for (Vertex<T>* v : this->vertexSet) {
        for (Edge<T>* edge : v->outgoing)
            edge->flow = 0;
        for (Edge<T>* edge : v->incoming)
            edge->flow = 0;
    }
}

template <class T>
void Graph<T>::testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) { //Tests the edge and visits the node
    if (!w->visited && residual > 0) {
        w->visited = true;
        w->path = e;
        q.push(w);
    }
}

#endif /* GRAPH_H_ */
