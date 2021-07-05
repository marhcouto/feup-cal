//
// Created by marhc on 17/05/2021.
//

#include "Graph.h"

// N O D E

Node::Node(NodeInfo in, int id): info(in), id(id) {}

Edge *Node::addEdge(Node *d, double w) {
    Edge *e = new Edge(this, d, w);
    adj.push_back(e);
    d->incoming.push_back(e);
    return e;
}

void Node::addEdge(Edge* edge) {
    adj.push_back(edge);
}

void Node::addIncoming(Edge *edge) {
    incoming.push_back(edge);
}

bool Node::operator<(const Node & Node) const {
    if (this->way == 2 && Node.way == 2)
        return this->distR < Node.distR;
    return this->dist < Node.dist;
}

NodeInfo& Node::getInfo() const {
    return (NodeInfo &) this->info;
}

double Node::getDist() const {
    return this->dist;
}

int Node::getQueueIndex() const {
    return this->queueIndex;
}

const Node *Node::getPath() const {
    return this->path;
}

const vector<Edge *> &Node::getIncoming() const {
    return incoming;
}

const std::vector<Edge *> &Node::getAdj() const {
    return adj;
}

Node *Node::getPair() const {
    return pair;
}

void Node::setPair(Node *pair) {
    this->pair = pair;
}

void Node::setVisited(bool visited) {
    this->visited = visited;
}

void Node::setPath(Node* path) {
    this->path = path;
}

bool Node::getVisited() const {
    return visited;
}

int Node::getNum() const {
    return num;
}

int Node::getLow() const {
    return low;
}

int Node::getId() const {
    return id;
}

int Node::getIDM() const {
    return idM;
}


// E D G E

Edge::Edge(Node *o, Node *d, double w): orig(o), dest(d), weight(w) {}

double Edge::getWeight() const {
    return weight;
}

Node *Edge::getOrig() const {
    return orig;
}

Node *Edge::getDest() const {
    return dest;
}


// G R A P H

std::vector<Node *> Graph::getNodeSet() const {
    return this->nodeSet;
}

bool Graph::addNode(Node *node) {
    Node *n = findNode(node->id);
    if (n != nullptr)
        return false;
    nodeSet.push_back(node);
    return true;
}

Node* Graph::addNode(const NodeInfo &in, int id) {
    Node *v = findNode(id);
    if (v != nullptr)
        return v;
    v = new Node(in, id);
    nodeSet.push_back(v);
    return v;
}

Edge* Graph::addEdge(Node* node1, Node* node2, double w) {
    Edge *e = new Edge(node1, node2, w);
    node2->addIncoming(e);
    node1->addEdge(e);
    return e;
}

Edge* Graph::addEdge(int id1, int id2, double w) {
    auto s = findNode(id1);
    auto d = findNode(id2);
    if (d == nullptr) throw NodeDoesNotExistException(id2, __func__, false);
    if (s == nullptr) throw NodeDoesNotExistException(id1, __func__, false);

    Edge *e = new Edge(s, d, w);
    s->addEdge(e);
    d->addIncoming(e);
    return e;
}

const vector<std::vector<double>> &Graph::getDistanceMatrix() const {
    return distanceMatrix;
}

const vector<std::vector<int>> &Graph::getPathsMatrix() const {
    return pathsMatrix;
}

Node* Graph::findNode(int id) const {
    for (auto v : nodeSet)
        if (v->id == id)
            return v;
    return nullptr;
}

Node* Graph::findNodeMatrixId(int idM) const {
    for (auto v : nodeSet)
        if (v->idM == idM)
            return v;
    return nullptr;
}

Graph::~Graph() {
    for (auto v : nodeSet) {
        for (auto edge : v->adj)
            delete edge;
        delete v;
    }
}


// A L G O R I T H M S

// Pre processing

void Graph::cleanGraph() {
    int i = 0;
    for (auto it = nodeSet.begin(); it != nodeSet.end();) {
        std::cout << ++i << std::endl;
        if ((*it)->adj.size() == 1 && (*it)->incoming.size() == 1) {
            double w1, w2;
            Node* n1, *n2;
            w1 = (*it)->adj.at(0)->weight;
            w2 = (*it)->incoming.at(0)->weight;
            n1 = (*it)->incoming.at(0)->orig;
            n2 = (*it)->adj.at(0)->dest;

            // Edge to itself
            if (n1->id == (*it)->id || n2->id == (*it)->id) {
                delete (*it);
                it = nodeSet.erase(it);
                continue;
            }

            // Eliminate edges
            for (auto it1 = n1->adj.begin(); it1 != n1->adj.end() || n1->id == 479437576;) {
                if ((*it1)->dest->id == (*it)->id) {
                    delete *it1;
                    it1 = n1->adj.erase(it1);
                    break;
                }
                else it1++;
            }
            for (auto it1 = n2->incoming.begin(); it1 != n2->incoming.end() || n2->id == 479437584;) {
                if ((*it1)->orig->id == (*it)->id) {
                    delete *it1;
                    it1 = n2->incoming.erase(it1);
                    break;
                }
                else it1++;
            }

            // New edge
            n1->addEdge(n2, w1 + w2);

            // Erase node
            delete (*it);
            it = nodeSet.erase(it);
        }
        else it++;
    }
}

// Route Calculation

std::vector<Node*> Graph::getRoute(int initialRange, std::vector<Node *> pickUps, Node* initialNode) {
    int range = initialRange;
    std::vector<Node*> allNodes;
    std::vector<Node*> res;

    for (Node* node : pickUps) {
        if (node->getInfo().getType() != PICKUP) std::cout << "NOT A PICKUP\n";
        node->available = true;
        node->pair->available = false;
        allNodes.push_back(node);
        allNodes.push_back(node->pair);
    }

    Node* nextNode = initialNode;
    res.push_back(nextNode);
    while (!allNodes.empty()) {
        double bestDistance = INF;
        int bestIndex;
        int currentIndex = 0;
        for (Node* node : allNodes) {
            if (!node->available) {
                currentIndex++;
                continue;
            }
            double possibleDistance;
            if (this->matrixCalculated) {
                possibleDistance = distanceMatrix[nextNode->idM][node->idM];
                if (possibleDistance < bestDistance) {
                    bestDistance = possibleDistance;
                    bestIndex = currentIndex;
                }
            } else {
                possibleDistance = bidirectionalDijkstra(nextNode->id, node->id);
                if (possibleDistance < bestDistance) {
                    bestDistance = possibleDistance;
                    bestIndex = currentIndex;
                }
            }
            currentIndex++;
        }
        if (bestDistance > range) {
            nextNode = findNearestRecharge(nextNode);
            range = initialRange;
        } else {
            range -= bestDistance;
            nextNode = allNodes[bestIndex];
            nextNode->pair->available = true;
            allNodes.erase(allNodes.begin() + bestIndex);
        }
        res.push_back(nextNode);
    }
    res.push_back(initialNode);
    return res;
}

Node* Graph::findNearestRecharge(Node *currentNode) { // A bit rusty

    for (Node* node : nodeSet)
        node->visited = false;

    std::queue<Node*> q;
    q.push(currentNode);
    while (true) {
        Node* node = q.front();
        node->visited = true;
        q.pop();
        for (Edge* edge : node->adj) {
            if (edge->dest->getInfo().getType() == RECHARGE) {
                return edge->dest;
            } else if (!edge->dest->visited) {
                q.push(edge->dest);
            }
        }
    }
}

// Shortest Path

double Graph::bidirectionalDijkstra(int start, int finish) {

    double bestDist = INF;

    Node* s = this->findNode(start);
    Node* p = this->findNode(finish);
    if (p == nullptr) throw NodeDoesNotExistException(finish, __func__, false);
    if (s == nullptr) throw NodeDoesNotExistException(start, __func__, false);

    for (Node *n : nodeSet) {
        n->visited = false;
        n->queueIndex = 0;
        n->dist = INF;
        n->distR = INF;
        n->way = 0;
        n->path = NULL;
    }

    s->way = 1;
    p->way = 2;

    MutablePriorityQueue<Node> nodeF;
    MutablePriorityQueue<Node> nodeB;

    s->dist = 0;
    p->distR = 0;
    nodeF.insert(s);
    nodeB.insert(p);

    bool found = false;

    while(!nodeF.empty() && !nodeB.empty()) {
        Node* node1 = nodeF.extractMin();
        Node* node2 = nodeB.extractMin();

        node1->visited = true;
        node2->visited = true;

        if(node1->dist + node2->distR >= bestDist && found)
            return bestDist;

        //FRONT
        for (Edge* edge : node1->adj) {
            double oldDist = edge->dest->dist;
            if (bestDist > edge->dest->distR + edge->weight + node1->dist && edge->dest->way == 2) {
                bestDist = edge->dest->distR + edge->weight + node1->dist;
                this->bidiCross1 = node1;
                this->bidiCross2 = edge->dest;
                found = true;
            }
            else if (!edge->dest->visited){
                edge->dest->way = 1;

                //MiniDijkstraStep
                if (edge->dest->dist > node1->dist + edge->weight) {
                    edge->dest->dist = node1->dist + edge->weight;
                    edge->dest->path = node1;

                    if(oldDist == INF)
                        nodeF.insert(edge->dest);
                    else
                        nodeF.decreaseKey(edge->dest);
                }
            }
        }

        //BACK
        for (Edge* edge : node2->incoming) {
            double oldDist = edge->orig->distR;
            if (bestDist > node2->distR + edge->weight + edge->orig->dist && edge->orig->way == 1) {
                bestDist = node2->distR + edge->weight + edge->orig->dist;
                this->bidiCross1 = edge->orig;
                this->bidiCross2 = node2;
                found = true;
            }
            else if (!edge->orig->visited){
                edge->orig->way = 2;

                //MiniDijkstraStep
                if (edge->orig->distR > node2->distR + edge->weight) {
                    edge->orig->distR = node2->distR + edge->weight;
                    edge->orig->path = node2;

                    if(oldDist == INF)
                        nodeB.insert(edge->orig);
                    else
                        nodeB.decreaseKey(edge->orig);
                }
            }
        }
    }

    return bestDist;
}

void Graph::dijkstraMulti() {

    this->matrixCalculated = true;

    std::cout << "Started Multi Dijkstra's\n";

    for (unsigned int i = 0; i < nodeSet.size(); i++) {
        std::vector<double> temp1;
        std::vector<int> temp2;
        for (unsigned int j = 0; j < nodeSet.size(); j++) {
            temp1.push_back(INF);
            temp2.push_back(0);
        }
        distanceMatrix.push_back(temp1); //Weights
        pathsMatrix.push_back(temp2); //Paths
    }

    for (unsigned int i = 0; i < nodeSet.size(); i++) {
        for (unsigned int j = 0; j < nodeSet.size(); j++) {
            distanceMatrix[i][j] = INF;
            pathsMatrix[i][j] = INTINF;
        }
    }
    std::cout << "I am still working\n";

    for (Node* node : nodeSet) {

        std::cout << "Another one:";
        std::cout << node->id << std::endl;

        for (Node* n : nodeSet) {
            n->visited = false;
            n->queueIndex = 0;
            n->dist = INF;
            n->path = NULL;
        }

        MutablePriorityQueue<Node> nodeQueue;
        distanceMatrix[node->getIDM()][node->getIDM()] = 0;
        node->dist = 0;
        nodeQueue.insert(node);
        while(!nodeQueue.empty()) {
            Node* node1 = nodeQueue.extractMin();
            node->visited = true;
            for (Edge* edge : node1->adj) {
                if (edge->dest->dist > node1->dist + edge->weight && !edge->dest->visited) {
                    double oldDist = edge->dest->dist;
                    edge->dest->dist = node1->dist + edge->weight;
                    edge->dest->path = node1;
                    distanceMatrix[node->getIDM()][edge->dest->getIDM()] = edge->dest->dist;
                    if (oldDist == INF) {
                        nodeQueue.insert(edge->dest);
                    }
                    else
                        nodeQueue.decreaseKey(edge->dest);
                }
            }
        }
        updatePaths(node);
    }
    std::cout << "Finished Multi Dijkstra's\n";
}

void Graph::updatePaths(Node* origin) {

    for (Node* node : nodeSet) {
        if (node->getIDM() == origin->getIDM()) {
            pathsMatrix[origin->getIDM()][origin->getIDM()] = origin->getIDM();
            continue;
        }
        Node* node1 = node;
        std::stack<int> st;
        while (node1->getIDM() != origin->getIDM()) {
            if (node1->path == NULL) break;
            st.push(node1->getIDM());
            node1 = node1->path;
        }
        int id1 = origin->getIDM();
        int id2 = node->getIDM();
        while (!st.empty()) {
            pathsMatrix[id1][node->getIDM()] = st.top();
            id1 = st.top();
            st.pop();
        }
    }
}

void Graph::floydWarshallShortestPath() { //Makes matrix with all paths

    std::cout << "Started Floyd Warhsall's\n";

    for (unsigned int i = 0; i < nodeSet.size(); i++) {
        std::vector<double> temp1;
        std::vector<int> temp2;
        for (unsigned int j = 0; j < nodeSet.size(); j++) {
            temp1.push_back(INF);
            temp2.push_back(0);
        }
        distanceMatrix.push_back(temp1); //Weights
        pathsMatrix.push_back(temp2); //Paths
    }

    for (Node* node : nodeSet) {
        distanceMatrix[node->getIDM()][node->getIDM()] = 0;
        pathsMatrix[node->getIDM()][node->getIDM()] = node->getIDM();
        for (Edge* edge : node->adj) { //Initial fill in
            distanceMatrix[node->idM][edge->dest->idM] = edge->weight;
            pathsMatrix[node->idM][edge->dest->idM] = edge->dest->idM;
        }
    }
    for (unsigned int i = 0; i < nodeSet.size(); i++) {
        for (unsigned int j = 0; j < nodeSet.size(); j++) {
            for (unsigned int k = 0; k < nodeSet.size(); k++) {
                if (distanceMatrix[j][k] > distanceMatrix[j][i] + distanceMatrix[i][k]) { //Checks if going through i makes the path shorter
                    distanceMatrix[j][k] = distanceMatrix[j][i] + distanceMatrix[i][k];
                    pathsMatrix[j][k] = pathsMatrix[j][i];
                }
            }
        }
    }
    std::cout << "Finished Floyd Warhsall's\n";
}

// Connectivity

void Graph::tarjan() {
    std::cout << "Started Tarjan's\n";

    std::stack<Node*> st;

    for (Node* n : nodeSet) {
        n->visited = false;
        n->isInStack = false;
    }

    int numCounter = 0;

    for (Node* n : nodeSet)
        dfsTarjan(n, numCounter, st);
}

int Graph::dfsTarjan(Node *node, int& counter, std::stack<Node*>& st) {
    if (node->visited)
        return node->num;
    node->visited = true;
    node->isInStack = true;
    st.push(node);
    counter++;
    node->low = counter;
    node->num = counter;

    for (Edge* edge : node->adj) {
        int temp = dfsTarjan(edge->dest, counter, st);
        if (temp < node->low && edge->dest->isInStack)
            node->low = temp;
    }
    if (node->low == node->num) {
        while(node != st.top()) {
            st.top()->low = node->low;
            st.top()->isInStack = false;
            st.pop();
        }
        st.top()->isInStack = false;
        st.pop();
    }
    return node->low;
    std::cout << "Finished tarjan's\n";
}

// Clustering

std::vector<std::vector<Node*>> Graph::clustering(std::vector<Node*> nodes, int noSets) {
    std::vector<std::vector<Node*>> clusters;
    for (Node* node : nodes) {
        std::vector<Node*> temp;
        temp.push_back(node);
        clusters.push_back(temp);
    }
    while (clusters.size() > noSets) {
        double bestDist = INF;
        int i1 = 0, j1 = 0;
        for (unsigned int i = 0; i < clusters.size(); i++) {
            for (unsigned int j = i + 1; j < clusters.size(); j++) {
                double possibleDist = getSetDistances(clusters[i], clusters[j]);
                if (bestDist > possibleDist ||
                    (bestDist == possibleDist && clusters[i].size() + clusters[j].size() < clusters[i1].size() + clusters[j1].size())) {
                    bestDist = possibleDist;
                    i1 = i;
                    j1 = j;
                }
            }
        }
        mergeSets(clusters, i1, j1);
    }
    return clusters;
}

double Graph::getSetDistances(std::vector<Node*> v1, std::vector<Node*> v2) {
    double bestDistance = INF;
    for (Node* n1 : v1) {
        for (Node* n2 : v2) {
            double possibleDistance;
            if (this->matrixCalculated)
                possibleDistance = (distanceMatrix[n1->getIDM()][n2->getIDM()] + distanceMatrix[n2->getIDM()][n1->getIDM()]) / 2;
            else
                possibleDistance = (bidirectionalDijkstra(n1->getId(), n2->getId()) + bidirectionalDijkstra(n2->getId(), n1->getId())) / 2;
            if (possibleDistance < bestDistance)
                bestDistance = possibleDistance;
        }
    }
    return bestDistance;
}

void Graph::mergeSets(std::vector<std::vector<Node*>>& clusters, int i, int j) {
    clusters[i].insert(clusters[i].end(), clusters[j].begin(), clusters[j].end());
    clusters.erase(clusters.begin() + j);
}


// UTILS FOR ALGORITHMS

void Graph::eliminateInaccessible(int id) {
    Node* node = findNode(id);
    if (node == nullptr) throw NodeDoesNotExistException(id, __func__, false);

    for (auto iterator = nodeSet.begin(); iterator != nodeSet.end();) {
        if ((*iterator)->low != node->low) {
            delete *iterator;
            iterator = nodeSet.erase(iterator);
        }
        else {
            for (auto it2 = (*iterator)->adj.begin(); it2 != (*iterator)->adj.end();) {
                if ((*it2)->dest->low != node->low) {
                    delete *it2;
                    it2 = (*iterator)->adj.erase(it2);
                }
                else {
                    it2++;
                }
            }
            iterator++;
        }
    }
}

void Graph::printMatrixes() {
    for (unsigned int i = 0; i < nodeSet.size(); i++) {
        for (unsigned int j = 0; j < nodeSet.size(); j++) {
            std::cout << distanceMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n\n\n";

    for (unsigned int i = 0; i < nodeSet.size(); i++) {
        for (unsigned int j = 0; j < nodeSet.size(); j++) {
            std::cout << pathsMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<int> Graph::multiGetPath(const std::vector<Node *> &nodes) {
    std::vector<int> res;
    res.push_back(nodes.at(0)->id);

    for (unsigned int i = 0; i < nodes.size() - 1; i++) {
        std::vector<int> tempPath;
        if (matrixCalculated)
            tempPath = getShortestPath(nodes.at(i), nodes.at(i + 1));
        else {
            bidirectionalDijkstra(nodes.at(i)->id, nodes.at(i + 1)->id);
            tempPath = getShortestPathBidirectional(nodes.at(i), nodes.at(i + 1));
        }
        res.insert(res.end(), tempPath.begin() + 1, tempPath.end());
    }
    return res;
}

std::vector<int> Graph::getShortestPath(Node *n1, Node *n2) {
    std::vector<int> res;

    res.push_back(n1->id);
    int nextI = n1->idM;

    while (n2->idM != nextI) {
        nextI = pathsMatrix[nextI][n2->idM];
        Node* node = findNodeMatrixId(nextI);
        if (node == nullptr) throw NodeDoesNotExistException(nextI, __func__, true);
        res.push_back(node->id);
    }

    return res;
}

std::vector<int> Graph::getShortestPath(int s, int d) const{ //Calculate the paths
    std::vector<int> res;
    Node* originV = this->findNode(s);
    Node* destV = this->findNode(d);
    if (destV == nullptr) throw NodeDoesNotExistException(d, __func__, false);
    if (originV == nullptr) throw NodeDoesNotExistException(s, __func__, false);

    res.push_back(originV->id);
    int nextI = originV->idM;

    while (destV->idM != nextI) {
        nextI = pathsMatrix[nextI][destV->idM];
        Node* node = findNodeMatrixId(nextI);
        if (node == nullptr) throw NodeDoesNotExistException(nextI, __func__, true);
        res.push_back(node->id);
    }

    return res;
}

std::vector<int> Graph::getShortestPathBidirectional(Node *n1, Node *n2) {
    std::vector<int> res;

    Node* node1 = this->bidiCross1;
    Node* node2 = this->bidiCross2;

    while(node1->id != n1->id) {
        res.insert(res.begin(), node1->id);
        node1 = node1->path;
    }
    while(node2->id != n2->id) {
        res.push_back(node2->id);
        node2 = node2->path;
    }

    res.insert(res.begin(),n1->id);
    res.push_back(n2->id);

    return res;
}

void Graph::assignIDM() {
    int counter = 0;
    for (Node* node : nodeSet) {
        node->idM = counter;
        counter++;
    }
}

