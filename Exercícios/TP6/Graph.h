/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <algorithm>
#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"

#define MAX_DIST 99999


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
    std::vector<std::vector<double> > D;
    std::vector<std::vector<int> > P;
public:
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    // Fp06 - single source
    void unweightedShortestPath(const T &s);
    void dijkstraShortestPath(const T &s);
    void bellmanFordShortestPath(const T &s);
    std::vector<T> getPath(const T &origin, const T &dest) const;

    // Fp06 - all pairs
    void floydWarshallShortestPath();
    std::vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

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
void Graph<T>::unweightedShortestPath(const T &orig) {
    Vertex<T> * origin = findVertex(orig);
    if (origin == NULL) return;

    for (Vertex<T> * v : vertexSet) {
        v->visited = false;
        v->dist = MAX_DIST;
        v->path = NULL;
    }
    std::queue<Vertex<T> *> q;

    q.push(origin);
    origin->visited = true;
    origin->dist = 0;

    Vertex<T> * aux;
    while (!q.empty()) {
        aux = q.front();
        q.pop();
        for (Edge<T> &edge : aux->adj) {
            if (!edge.dest->visited) {
                q.push(edge.dest);
                edge.dest->visited = true;
                edge.dest->dist = 1 + aux->dist;
                edge.dest->path = aux;
            }
        }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    Vertex<T> * ori = findVertex(origin);
    if (ori == NULL) return;

    for (Vertex<T> * v : vertexSet) {
        v->dist = MAX_DIST;
        v->path = NULL;
    }
    ori->dist = 0;
    MutablePriorityQueue<Vertex<T> > q;
    q.insert(ori);

    Vertex<T> * aux;
    while (!q.empty()) {
        aux = q.extractMin();
        for (Edge<T> &edge : aux->adj) {
            if (edge.dest->dist > aux->dist + edge.weight) {
                double old = edge.dest->dist;
                edge.dest->dist = aux->dist + edge.weight;
                edge.dest->path = aux;
                if (old == MAX_DIST)
                    q.insert(edge.dest);
                else
                    q.decreaseKey(edge.dest);
            }
        }
    }
}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
    Vertex<T> * ori = findVertex(orig);
    if (ori == NULL) return;

    for (Vertex<T> * v : vertexSet) {
        v->dist = MAX_DIST;
        v->path = NULL;
    }
    ori->dist = 0;
    for (int i = 1; i < vertexSet.size(); ++i) {
        for (Vertex<T> *v : vertexSet) {
            for (Edge<T> &edge : v->adj) {
                if (edge.dest->dist > v->dist + edge.weight) {
                    edge.dest->dist = v->dist + edge.weight;
                    edge.dest->path = v;
                }
            }
        }
    }
}


template<class T>
std::vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
    std::vector<T> res;
    Vertex<T> *a = findVertex(origin), *b = findVertex(dest);
    if (a == NULL || b == NULL) return res;

    while (a != b) {
        res.insert(res.begin(), b->info);
        b = b->path;

        if (b == NULL) {
            res.clear();
            return res;
        }
    }
    res.insert(res.begin(), a->info);
    return res;
}



/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    D = std::vector<std::vector<double> >(vertexSet.size(), std::vector<double>(vertexSet.size(), 0));
    P = std::vector<std::vector<int> >(vertexSet.size(), std::vector<int>(vertexSet.size(), 0));

    double w;
    Vertex<T> * v;
    for (int i = 0; i < vertexSet.size(); ++i) {
        v = vertexSet[i];
        for (int j = 0; j < vertexSet.size(); ++j) {
            if (i == j) {
                D[i][j] = 0;
                //continue;
            }
            w = INF;
            for (Edge<T> &edge : v->adj) {
                if (edge.dest == vertexSet[j]) {
                    w = edge.weight;
                    break;
                }
            }
            D[i][j] = w;
            if (w != INF) P[i][j] = j;
        }
    }

    for (int k = 0; k < vertexSet.size(); ++k) {
        for (int i = 0; i < vertexSet.size(); ++i) {
            for (int j = 0; j < vertexSet.size(); ++j) {
                if (D[i][j] > D[i][k] + D[k][j]) {
                    D[i][j] = D[i][k] + D[k][j];
                    P[i][j] = P[i][k];
                }
            }
        }
    }
}

template<class T>
std::vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
    std::vector<T> res;
    Vertex<T> * o = findVertex(orig);
    Vertex<T> * d = findVertex(dest);
    if (o == NULL or d == NULL) return res;

    auto oIt = std::find(vertexSet.begin(), vertexSet.end(), o);
    auto dIt = std::find(vertexSet.begin(), vertexSet.end(), d);

    int oIdx = std::distance(vertexSet.begin(), oIt);
    int dIdx = std::distance(vertexSet.begin(), dIt);
    if (D[oIdx][dIdx] == INF) return res;

    int currentIdx = oIdx;
    while (currentIdx != dIdx) {
        res.push_back(vertexSet[currentIdx]->info);
        currentIdx = P[currentIdx][dIdx];
    }

    res.push_back(vertexSet[dIdx]->info);
    return res;
}


#endif /* GRAPH_H_ */
