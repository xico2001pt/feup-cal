#include "graphviewer.h"
#include <iostream>
#include <fstream>

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void readNodes(GraphViewer &gv, const std::string &filename) {
    std::ifstream f(filename);
    int n, id, x, y;

    f >> n;
    for (int i = 0; i < n; ++i) {
        f >> id >> x >> y;
        gv.addNode(id, sf::Vector2f(x, y));
    }
}

void readEdges(GraphViewer &gv, const std::string &filename) {
    std::ifstream f(filename);
    int e, id, from, to;

    f >> e;
    for (int i = 0; i < e; ++i) {
        f >> id >> from >> to;
        gv.addEdge(id, gv.getNode(from), gv.getNode(to));
    }
}

void ex3() {
    // Instantiate GraphViewer
    GraphViewer gv;
    readNodes(gv, "./resources/map1/nodes.txt");
    readEdges(gv, "./resources/map1/edges.txt");

    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    // Create window
    gv.createWindow(600, 600);

    // Join viewer thread (blocks till window closed)
    gv.join();
}
