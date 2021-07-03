#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex1() {
    // Instantiate GraphViewer
    GraphViewer gv;

    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    // Create nodes
    Node &node0 = gv.addNode(0, sf::Vector2f(200, 300));
    Node &node1 = gv.addNode(1, sf::Vector2f(400, 300));
    Node &node2 = gv.addNode(2, sf::Vector2f(500, 300));
    //gv.removeNode(1);
    node2.setLabel("This is a vertex");
    node2.setColor(sf::Color::Green);

    // Create edges
    Edge &edge0 =gv.addEdge(0, node0, node1, GraphViewer::Edge::DIRECTED);
    Edge &edge1 =gv.addEdge(1, node0, node2, GraphViewer::Edge::DIRECTED);
    edge1.setColor(sf::Color::Black);
    edge1.setLabel("This is a edge");
    for (Edge * edge : gv.getEdges())
        edge->setColor(sf::Color::Yellow);

    // Set background
    gv.setBackground("../TP7_graphviewer/resources/background.png");

    // Create window
    gv.createWindow(600, 600);

    // Join viewer thread (blocks till window closed)
    gv.join();
}
