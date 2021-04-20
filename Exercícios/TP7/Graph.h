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

#define MAX_DIST 999999

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
	int queueIndex = 0;
public:
	Edge(Vertex<T> *o, Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
    friend class MutablePriorityQueue<Edge<T>>;

	double getWeight() const;
	Vertex<T> *getOrig() const;
	Vertex<T> *getDest() const;
	bool operator<(const Edge<T> &edge) const;
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

template <class T>
bool Edge<T>::operator<(const Edge<T> &edge) const {
    return this->weight < edge.getWeight();
}

/*************************** Graph  **************************/

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

	// Fp07 (Kruskal's algorithm)
	void makeSet(Vertex<T> * x);
	Vertex<T> * findSet(Vertex<T> * x);
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
	Vertex<T> * origin = findVertex(sourc);
    Vertex<T> * destination = findVertex(dest);
    if (origin == nullptr || destination == nullptr) return false;

	Edge<T> * edge1 = origin->addEdge(destination, w);
    Edge<T> * edge2 = destination->addEdge(origin, w);
    if (edge1 == nullptr || edge2 == nullptr) return false;

    edge1->reverse = edge2;
    edge2->reverse = edge1;
	return true;
}

template <class T>
Graph<T>::~Graph() {
    /*
	for (auto v : vertexSet) {
		for (auto e : v->adj)
			delete e;
		delete v;
	}
    */
}

/**************** Minimum Spanning Tree  ***************/

template <class T>
std::vector<Vertex<T>* > Graph<T>::calculatePrim() {
    if (this->vertexSet.empty()) return {};

    for (Vertex<T> * v : this->vertexSet) {
        v->visited = false;
        v->dist = INF;
        v->path = nullptr;
    }

    Vertex<T> * v0 = this->vertexSet[0];
    v0->visited = true;
    v0->dist = 0;

    MutablePriorityQueue<Vertex<T> > q;
    q.insert(v0);

    Vertex<T> * aux;
    while (!q.empty()) {
        aux = q.extractMin();
        for (Edge<T> * edge : aux->adj) {
            if (edge->dest->dist > aux->dist + edge->weight) {
                edge->dest->dist = aux->dist + edge->weight;
                edge->dest->path = aux;
                if (!edge->dest->visited) {
                    edge->dest->visited = true;
                    q.insert(edge->dest);
                }
                else
                    q.decreaseKey(edge->dest);
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
void Graph<T>::linkSets(Vertex<T> * x, Vertex<T> * y) {
	if (x->rank > y->rank)
		y->path = x;
	else {
		x->path = y;
		if (x->rank == y->rank)
			y->rank++;
	}
}

template <class T>
Vertex<T> * Graph<T>::findSet(Vertex<T> * x) {
	if (x != x->path)
		x->path = findSet(x->path);
	return x->path;
}

struct {
    template<class T>
    bool operator()(Edge<T> * e1, Edge<T> * e2) const { return e1->getWeight() < e2->getWeight(); }
} compare;

/**
 * Implementation of Kruskal's algorithm to find a minimum
 * spanning tree of an undirected connected graph (edges added with addBidirectionalEdge).
 * It is used a disjoint-set data structure to achieve a running time O(|E| log |V|).
 * The solution is defined by the "path" field of each vertex, which will point
 * to the parent vertex in the tree (nullptr in the root).
 */
template <class T>
std::vector<Vertex<T>*> Graph<T>::calculateKruskal() {
    if (this->vertexSet.empty()) return {};

    MutablePriorityQueue<Edge<T> > edges;

    for (Vertex<T> * v : vertexSet) {
        v->visited = false;
        makeSet(v);
        for (Edge<T> * edge : v->adj) {
            edge->selected = false;
            edges.insert(edge);
        }
    }

    Edge<T> * edge;
    Vertex<T> * setOri, * setDest;
    while (!edges.empty()) {
        edge = edges.extractMin();
        setOri = findSet(edge->getOrig());
        setDest = findSet(edge->getDest());
        if (setOri != setDest) {
            linkSets(setOri, setDest);
            edge->selected = true;
            edge->reverse->selected = true;
        }
    }

    vertexSet[0]->path = nullptr;
    dfsKruskalPath(vertexSet[0]);

    return vertexSet;
}

/**
 * Auxiliary function to set the "path" field to make a spanning tree.
 */
template <class T>
void Graph<T>::dfsKruskalPath(Vertex<T> *v) {
    v->visited = true;
    for (Edge<T> * edge : v->adj) {
        if (!edge->getDest()->visited && edge->selected) {
            edge->getDest()->path = edge->getOrig();
            dfsKruskalPath(edge->getDest());
        }
    }
}

#endif /* GRAPH_H_ */
