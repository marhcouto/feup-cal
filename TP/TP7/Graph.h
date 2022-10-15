/*
 * Graph.h.
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include "MutablePriorityQueue.h"

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
    friend double spanningTreeCost(const std::vector<Vertex<int>*> &);
	T info;                 // contents
	std::vector<Edge<T> *> adj;  // outgoing edges

	bool visited;
	double dist = 0;
	Vertex<T> *path = nullptr;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	// Fp07 - minimum spanning tree (Kruskal)
	int id;
	int rank;

	Edge<T> * addEdge(Vertex<T> *dest, double w);
public:
	Vertex(T in);
	bool operator<(const Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
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
Edge<T> *Vertex<T>::addEdge(Vertex<T> *d, double w) {
	Edge<T> *e = new Edge<T>(this, d, w);
	adj.push_back(e);
	return e;
}

template <class T>
bool Vertex<T>::operator<(const Vertex<T> & vertex) const {
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
	Vertex<T> *orig; 		    // Fp07
	Vertex<T> * dest;           // destination vertex
	double weight;              // edge weight
	bool selected = false;      // Fp07
	Edge<T> *reverse = nullptr; // Fp07
public:
	Edge(Vertex<T> *o, Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;

	double getWeight() const;
	Vertex<T> *getOrig() const;
	Vertex<T> *getDest() const;
};

template <class T>
Edge<T>::Edge(Vertex<T> *o, Vertex<T> *d, double w): orig(o), dest(d), weight(w) {}

template <class T>
double Edge<T>::getWeight() const {
	return weight;
}

template <class T>
Vertex<T> *Edge<T>::getOrig() const {
	return orig;
}

template <class T>
Vertex<T> *Edge<T>::getDest() const {
	return dest;
}

/*************************** Graph  **************************/

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

	// Fp07 (Kruskal's algorithm)
	void makeSet(Vertex<T> * x);
	Vertex<T> * findSet(Vertex<T> * x);
    Vertex<T>* findSet2(Vertex<T> * x);
	void linkSets(Vertex<T> * x, Vertex<T> * y);
	void dfsKruskalPath(Vertex<T> *v);


public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool addBidirectionalEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;
	~Graph();

	// Fp07 - minimum spanning tree
	std::vector<Vertex<T>*> calculatePrim();
	std::vector<Vertex<T>*> calculateKruskal();
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
	return nullptr;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if (findVertex(in) != nullptr)
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
	if (v1 == nullptr || v2 == nullptr)
		return false;
	v1->addEdge(v2, w);
	return true;
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr) return false;

    auto edge1 = v1->addEdge(v2, w);
    auto edge2 = v2->addEdge(v1, w);

    edge1->reverse = edge2;
    edge2->reverse = edge1;

    return true;
}

template <class T>
Graph<T>::~Graph() {

	for (auto v : vertexSet) {
		for (auto e : v->adj)
			delete e;
		delete v;
	}

}

/**************** Minimum Spanning Tree  ***************/

template <class T>
std::vector<Vertex<T>* > Graph<T>::calculatePrim() { //Altered dijkstra, evaluation is made through comparison with the edge's weight

    for (auto& v : vertexSet) {
        v->dist = INF;
        v->path = nullptr;
        v->visited = false;
        v->queueIndex = 0;
    }

    MutablePriorityQueue<Vertex<T>> vertexQueue;
    vertexQueue.insert(vertexSet.at(0));

    while(!vertexQueue.empty()) {
        Vertex<T>* v = vertexQueue.extractMin();
        v->visited = true;
        for (Edge<T>* edge : v->adj) {
            if (edge->dest->dist > edge->weight && !edge->dest->visited) {
                double oldDist = edge->dest->dist;
                edge->dest->dist = edge->weight;
                edge->dest->path = v;
                if (oldDist == INF)
                    vertexQueue.insert(edge->dest);
                else
                    vertexQueue.decreaseKey(edge->dest);
            }
        }
    }
    return vertexSet;
}

/**
 * Disjoint sets operations (page 571, Introduction to Algorithms) for Kruskal's algorithm.
 */

template <class T>
void Graph<T>::makeSet(Vertex<T> * x) {
	x->path = x;
	x->rank = 0;
}

template <class T>
void Graph<T>::linkSets(Vertex<T> * x, Vertex<T> * y) { //Does not work
    if (x->rank > y->rank) {
        y->path->path = x;
    }
    else {
        x->path->path = y;
        if (x->rank == y->rank)
            y->rank++;
    }
}

template <class T>
Vertex<T> * Graph<T>::findSet2(Vertex<T> * x) {
    if (x != x->path)
        return findSet2(x->path);
    return x->path;
}

template <class T>
Vertex<T> * Graph<T>::findSet(Vertex<T> * x) { //Couldn't get around it
	if (x != x->path)
		x->path = findSet(x->path);
	return x->path;
}

/**
 * Implementation of Kruskal's algorithm to find a minimum
 * spanning tree of an undirected connected graph (edges added with addBidirectionalEdge).
 * It is used a disjoint-set data structure to achieve a running time O(|E| log |V|).
 * The solution is defined by the "path" field of each vertex, which will point
 * to the parent vertex in the tree (nullptr in the root).
 */
template <class T>
std::vector<Vertex<T>*> Graph<T>::calculateKruskal() {

    //FIRST WAY, LESS COMPLICATED CODE, SLOWER
    /*
    unsigned int counter = 0;
    for (Vertex<T>* v : vertexSet) {
        makeSet(v); //Disjoint set with only 1 element
        v->id = counter++; //To use to make sure we don't add repeated edges up ahead
    }

    std::vector<Edge<T>*> edges;

    //Putting edges in a vector
    for (Vertex<T>* v : vertexSet) {
        for (Edge<T>* edge : v->adj) {
            if (edge->orig->id < edge->dest->id) //To make sure edges are not repeated
                edges.push_back(edge);
        }
    }

    //Sorting edges by weight
    std::sort(edges.begin(), edges.end(), [](Edge<T>* edge1, Edge<T>* edge2) {return edge1->weight < edge2->weight;});

    //Stop when we have the minimum amount of edges (|V| - 1)
    unsigned int edgeCounter = 0;

    for (Edge<T>* edge : edges) {
        if (findSet2(edge->orig) != findSet2(edge->dest)) {
            findSet2(edge->dest)->path = edge->orig; //Link the disjoint sets
            edgeCounter++;
            if (edgeCounter == vertexSet.size() - 1) break;
        }
    }
    */

    //OFFICIAL WAY

    unsigned int counter = 0;
    for (Vertex<T>* v : vertexSet) {
        makeSet(v); //Disjoint set with only 1 element
        v->id = counter++; //To use to make sure we don't add repeated edges up ahead
    }

    std::vector<Edge<T>*> edges;

    //Putting edges in a vector
    for (Vertex<T>* v : vertexSet) {
        for (Edge<T>* edge : v->adj) {
            edge->selected = false;
            if (edge->orig->id < edge->dest->id) //To make sure edges are not repeated
                edges.push_back(edge);
        }
    }

    //Sorting edges by weight
    std::sort(edges.begin(), edges.end(), [](Edge<T>* edge1, Edge<T>* edge2) {return edge1->weight < edge2->weight;});

    //Stop when we have the minimum amount of edges (|V| - 1)
    unsigned int edgeCounter = 0;

    for (Edge<T>* edge : edges) {
        if (findSet(edge->orig) != findSet(edge->dest)) {
            linkSets(edge->orig, edge->dest);
            edge->selected = true;
            edge->reverse->selected = true;
            edgeCounter++;
            if (edgeCounter == vertexSet.size() - 1) break;
        }
    }

    for (Vertex<T>* vertex : vertexSet) {
        vertex->visited = false;
    }

    vertexSet.at(0)->path = nullptr;

    dfsKruskalPath(vertexSet.at(0));

    return vertexSet;
}

/**
 * Auxiliary function to set the "path" field to make a spanning tree.
 */
template <class T>
void Graph<T>::dfsKruskalPath(Vertex<T> *v) {
    //IRRELEVANT FOR THE FIRST WAY
    v->visited = true;

    for (Edge<T>* edge : v->adj) {
        if (edge->selected && !edge->dest->visited) {
            edge->dest->path = v;
            dfsKruskalPath(edge->dest);
        }
    }
}

#endif /* GRAPH_H_ */
