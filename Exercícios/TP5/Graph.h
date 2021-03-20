/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>

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
    if (findVertex(in) != NULL) return false;
    vertexSet.push_back(new Vertex<T>(in));
    return false;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    Vertex<T> *v1 = findVertex(sourc), *v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL) return false;
    v1->addEdge(v2, w);
    return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    adj.push_back(Edge<T>(d, w));
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T> *v1 = findVertex(sourc), *v2 = findVertex(dest);
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
    typename std::vector<Edge<T>>::iterator it = adj.begin();
    while (it != adj.end()) {
        if (it->dest == d) {
            adj.erase(it);
            return true;
        }
        ++it;
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
    Vertex<T> *v = findVertex(in);
    if (v == NULL) return false;

    typename std::vector<Vertex<T> *>::iterator it = vertexSet.begin();
    while (it != vertexSet.end()) {
        if (*it == v) {
            it = vertexSet.erase(it);
        }
        else {
            (*it)->removeEdgeTo(v);
            ++it;
        }
    }
    delete v;
    return true;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
std::vector<T> Graph<T>::dfs() const {
    std::vector<T> res;
    for (Vertex<T> *v : vertexSet)
        v->visited = false;
    for (Vertex<T> *v : vertexSet)
        if (!v->visited)
            dfsVisit(v, res);
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> & res) const {
    v->visited = true;
    res.push_back(v->info);
    for (const Edge<T> &edge : v->adj)
        if (!edge.dest->visited)
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
std::vector<T> Graph<T>::bfs(const T & source) const {
    std::vector<T> res;
    Vertex<T> *s = findVertex(source);
    if (s == NULL) return res;

    for (Vertex<T> *v : vertexSet)
        v->visited = false;
    std::queue<Vertex<T>*> q;
    s->visited = true;
    q.push(s);

    Vertex<T> *v;
    while (!q.empty()) {
        v = q.front();
        q.pop();
        for (Edge<T> &edge : v->adj) {
            if (!edge.dest->visited) {
                q.push(edge.dest);
                edge.dest->visited = true;
            }
        }
        res.push_back(v->info);
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
    std::vector<T> res;

    for (Vertex<T> *v : vertexSet)
        v->indegree = 0;
    for (Vertex<T> *v : vertexSet) {
        for (Edge<T> &edge : v->adj) {
            ++edge.dest->indegree;
        }
    }

    std::queue<Vertex<T>*> q;
    for (Vertex<T> *v : vertexSet) {
        if (v->indegree == 0)
            q.push(v);
    }

    while (!q.empty()) {
        Vertex<T> *v = q.front();
        q.pop();
        res.push_back(v->info);
        for (Edge<T> &edge : v->adj) {
            --edge.dest->indegree;
            if (edge.dest->indegree == 0)
                q.push(edge.dest);
        }
    }
    if (res.size() != vertexSet.size()) return std::vector<T>();
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
    Vertex<T> *s = findVertex(source);
    if (s == NULL) return 0;

    int result = 0;
    Vertex<T> *best;
    for (Edge<T> &e : s->adj) {
        for (Vertex<T> *ve : vertexSet)
            ve->visited = false;
        std::queue<Vertex<T>*> q;
        s->visited = true;
        for (Edge<T> &ed : s->adj)
            ed.dest->visited = true;
        q.push(e.dest);

        Vertex<T> *v;
        int counter = -1;
        while (!q.empty()) {
            v = q.front();
            q.pop();
            for (Edge<T> &edge : v->adj) {
                if (!edge.dest->visited) {
                    q.push(edge.dest);
                    edge.dest->visited = true;
                }
            }
            ++counter;
        }
        if (counter > result) {
            result = counter;
            best = e.dest;
        }
    }
    inf = best->info;
    return result;
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
    for (Vertex<T> *v : vertexSet) {
        v->visited = false;
        v->processing = false;
    }
    for (Vertex<T> *v : vertexSet) {
        if (!v->visited) {
            if (!dfsIsDAG(v)) return false;
        }
    }
    return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
    v->visited = true;
    v->processing = true;
    for (const Edge<T> &edge : v->adj) {
        if (edge.dest->processing) return false;
        if (!edge.dest->visited) {
            if (!dfsIsDAG(edge.dest)) return false;
        }
    }
    v->processing = false;
    return true;
}

#endif /* GRAPH_H_ */
