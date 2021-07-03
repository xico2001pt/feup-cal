#include <fstream>

#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex4() {
    GraphViewer gv;
    gv.setScale(1.0/4000.0);
    gv.setCenter(sf::Vector2f(-8.600, -41.146));

    ifstream iNodesStream("../TP7_graphviewer/resources/map2/nodes.txt");
    GraphViewer::id_t N, idNode; double lat, lon;
    iNodesStream >> N;
    for(GraphViewer::id_t i = 0; i < N; ++i){
        iNodesStream >> idNode >> lat >> lon;
        Node &node = gv.addNode(idNode, sf::Vector2f(lon, -lat));
        node.setOutlineThickness(0.00002);
        node.setSize(0.0001);
    }

    ifstream iEdgesStream("../TP7_graphviewer/resources/map2/edges.txt");
    GraphViewer::id_t E, idEdge, u, v;
    iEdgesStream >> E;
    for(GraphViewer::id_t i = 0; i < E; ++i){
        iEdgesStream >> idEdge >> u >> v;
        Edge &edge = gv.addEdge(idEdge, gv.getNode(u), gv.getNode(v));
        edge.setThickness(0.0001);
        edge.setColor(GraphViewer::WHITE);
    }

    gv.setBackground(
            "../TP7_graphviewer/resources/map2/map.jpg",
            sf::Vector2f(-8.7817, -41.3095),
            sf::Vector2f(1.3297, 1.0)/7010.0f,
            0.8
    );

    // TODO

    gv.createWindow(1600, 900);
    gv.join();
}

