/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <iostream>
#include <stack>

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	std::vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	bool processing;       // auxiliary field used by isDAG
	int indegree;          // auxiliary field used by topsort

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	friend class Graph<T>;
};

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
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T> *v,  std::vector<T> & res) const;
	Vertex<T> *findVertex(const T &in) const;
	bool dfsIsDAG(Vertex<T> *v) const;
public:
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeEdge(const T &sourc, const T &dest);
    std::vector<T> dfs() const;
    std::vector<T> bfs(const T &source) const;
    std::vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
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

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {

    //Check if vertex exists
    if (this->findVertex(in) != NULL) {
        return false;
    }

    Vertex<T>* v = new Vertex<T>(in);
    this->vertexSet.push_back(v);
    return true;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {

    Vertex<T>* v1 = this->findVertex(sourc);
    Vertex<T>* v2 = this->findVertex(dest);

    //Check if vertexes exist
    if (v1 == NULL || v2 == NULL)
        return false;

    v1->addEdge(v2, w);
    return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    Edge<T> e(d, w);
    this->adj.push_back(e);
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {

    Vertex<T>* v1 = this->findVertex(sourc);
    Vertex<T>* v2 = this->findVertex(dest);

    //Check if vertexes exist
    if (v1 == NULL || v2 == NULL) return false;
    return v1->removeEdgeTo(v2);

}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {

    for (typename std::vector<Edge<T> >::iterator it = this->adj.begin(); it != this->adj.end(); it++) {
        if (it->dest == d) {
            this->adj.erase(it);
            return true;
        }
    }
    return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {

    for (auto it = vertexSet.begin(); it != this->vertexSet.end(); it++) {
        if ((*it)->info == in) {

            //Remove edges to this vertex
            for (auto it2 = vertexSet.begin(); it2 != this->vertexSet.end(); it2++) {
                (*it2)->removeEdgeTo(*it);
            }

            //Delete it
            delete *it;
            this->vertexSet.erase(it);
            return true;
        }
    }
    return false;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
std::vector<T> Graph<T>::dfs() const { //nodes to the bottom are analysed first (child before brother)

    std::vector<T> res;

    //Set all visited to false
    for (auto& vertex : this->vertexSet) {
        vertex->visited = false;
    }
    //Explore the vector
    for (auto& vertex : this->vertexSet) {
        this->dfsVisit(vertex, res);
    }
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> & res) const {

    //Base case
    if (v->visited) return;

    v->visited = true;
    res.push_back(v->info);

    //Recursive step
    for (auto edge : v->adj)
        dfsVisit(edge.dest, res);
}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
std::vector<T> Graph<T>::bfs(const T & source) const { //nodes to the side are analysed first (brother before child)

    for (auto& vertex : this->vertexSet)
        vertex->visited = false;

    std::vector<T> res;
    std::queue<Vertex<T>*> toVisit;
    Vertex<T>* v = this->findVertex(source); //Starting vertex

    v->visited = true;
    res.push_back(v->info);
    toVisit.push(v); //Queue

    while (!toVisit.empty()) {
        for (auto edge : toVisit.front()->adj) { //Pushes adjacent nodes to queue
            if (edge.dest->visited)
                continue;
            res.push_back(edge.dest->info);
            toVisit.push(edge.dest);
            edge.dest->visited = true;
            std::cout << edge.dest->info << std::endl;
        }
        toVisit.pop();
    }

    return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template<class T>
std::vector<T> Graph<T>::topsort() const {

    std::queue<Vertex<T>*> q;
    std::vector<T> res;

    for (auto& vertex : this->vertexSet)
        vertex->indegree = 0;

    //Set indegrees for the graph's vertexes
    for (auto& vertex : this->vertexSet) {
        for (auto edge : vertex->adj) {
            edge.dest->indegree++;
        }
    }

    //Construct initial queue
    for (auto& vertex : this->vertexSet) {
        if (vertex->indegree == 0) {
            q.push(vertex);
        }
    }

    while (!q.empty()) {

        //Lower indegree of adjacent vertexes
        for (auto edge : (q.front())->adj) {
            edge.dest->indegree--;
            if (edge.dest->indegree == 0)
                q.push(edge.dest);
        }

        res.push_back((q.front())->info);
        q.pop();
    }

    //Means its a ciclical graph
    if (res.size() != this->vertexSet.size())
        res.clear();

    return res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T>
int Graph<T>::maxNewChildren(const T & source, T &inf) const {
    int nChild = 0;

    for (auto& vertex : this->vertexSet)
        vertex->visited = false;

    std::vector<T> res;
    std::queue<Vertex<T>*> toVisit;
    Vertex<T>* v = this->findVertex(source);

    v->visited = true;
    res.push_back(v->info);
    toVisit.push(v);

    while (!toVisit.empty()) {
        int n = 0;
        for (auto edge : toVisit.front()->adj) {
            n++;
            if (edge.dest->visited)
                continue;
            res.push_back(edge.dest->info);
            toVisit.push(edge.dest);
            edge.dest->visited = true;
            if (n > nChild) {
                nChild = n;
                inf = toVisit.front()->info;
            }
        }
        toVisit.pop();
    }

    return nChild;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template <class T>
bool Graph<T>::isDAG() const {

    bool returnVal = false;

    //Set all visited to false
    for (auto& vertex : this->vertexSet) {
        vertex->visited = false;
        vertex->processing = false;
    }

    //Explore the vector
    for (auto& vertex : this->vertexSet)
        returnVal |= this->dfsIsDAG(vertex);

    return returnVal;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
    //Base case
    if (v->processing) return true;
    if (v->visited) return false;

    bool returnVal = false;
    v->visited = true;
    v->processing = true;

    //Recursive step
    for (auto edge : v->adj)
        returnVal |= dfsIsDAG(edge.dest);

    v->processing = false;

    return returnVal;
}

#endif /* GRAPH_H_ */
