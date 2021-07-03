#include "graphviewer.h"
#include <Windows.h>

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex2() {
    // Instantiate GraphViewer
    GraphViewer gv;

    // Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    // Create nodes (by Adriano Soares)
    Node &node0 = gv.addNode(0, sf::Vector2f(300, 50));
    Node &node1 = gv.addNode(1, sf::Vector2f(318, 58));
    Node &node2 = gv.addNode(2, sf::Vector2f(325, 75));
    Node &node3 = gv.addNode(3, sf::Vector2f(318, 93));
    Node &node4 = gv.addNode(4, sf::Vector2f(300, 100));
    Node &node5 = gv.addNode(5, sf::Vector2f(282, 93));
    Node &node6 = gv.addNode(6, sf::Vector2f(275, 75));
    Node &node7 = gv.addNode(7, sf::Vector2f(282, 58));
    Node &node8 = gv.addNode(8, sf::Vector2f(150, 200));
    Node &node9 = gv.addNode(9, sf::Vector2f(300, 200));
    Node &node10 = gv.addNode(10, sf::Vector2f(450, 200));
    Node &node11 = gv.addNode(11, sf::Vector2f(300, 400));
    Node &node12 = gv.addNode(12, sf::Vector2f(200, 550));
    Node &node13 = gv.addNode(13, sf::Vector2f(400, 550));

    // Create nodes (by Adriano Soares)
    Edge &edge0 = gv.addEdge(0,node0,node1);
    Edge &edge1 = gv.addEdge(1,node1,node2);
    Edge &edge2 = gv.addEdge(2,node2,node3);
    Edge &edge3 = gv.addEdge(3,node3,node4);
    Edge &edge4 = gv.addEdge(4,node4,node5);
    Edge &edge5 = gv.addEdge(5,node5,node6);
    Edge &edge6 = gv.addEdge(6,node6,node7);
    Edge &edge7 = gv.addEdge(7,node7,node0);
    Edge &edge8 = gv.addEdge(8,node4,node9);
    Edge &edge9 = gv.addEdge(9,node9,node8);
    Edge &edge10 = gv.addEdge(10,node9,node10);
    Edge &edge11 = gv.addEdge(11,node9,node11);
    Edge &edge12 = gv.addEdge(12,node11,node12);
    Edge &edge13 = gv.addEdge(13,node11,node13);

    // Create window
    gv.createWindow(600, 600);

    for (int i = 0; i < 10; ++i) {
        Sleep(500);
        gv.lock();
        if (i % 2 == 0) {
            node12.setPosition(sf::Vector2f(250, 550));
            node13.setPosition(sf::Vector2f(350, 550));
        }
        else {
            node12.setPosition(sf::Vector2f(200, 550));
            node13.setPosition(sf::Vector2f(400, 550));
        }
        gv.unlock();
    }

    // Join viewer thread (blocks till window closed)
    gv.join();
}

