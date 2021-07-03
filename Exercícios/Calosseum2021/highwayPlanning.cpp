#include <iostream>

class subset {
public:
    int parent, rank;
};

class Road {
public:
    int src, dest, weight;
};

int myComp(const void *a, const void *b) {
    Road *a1 = (Road*) a;
    Road *b1 = (Road*) b;
    if (a1->weight < b1->weight) return -1;
    if (a1->weight > b1->weight) return 1;
    return 0;
}

static int V, E;
static Road roads[3000];
static subset subsets[99];
static Road result[99];
static int bestRoadsIdx[99];

int find(int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets[i].parent);
    return subsets[i].parent;
}

void Union(int x, int y) {
    int xroot = find(x);
    int yroot = find(y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int KruskalMST() {
    int i = 0;
    int e = 0;

    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < V - 1 && i < E) {
        Road nextRoad = roads[i++];

        int x = find(nextRoad.src);
        int y = find(nextRoad.dest);

        if (x != y) {
            bestRoadsIdx[e] = i;
            result[e++] = nextRoad;
            Union(x, y);
        }
    }

    int minimumCost = 0;
    for (i = 0; i < e; ++i)
        minimumCost += result[i].weight;
    return minimumCost;
}

int SecondKruskalMST(int exclude) {
    int i = 0;
    int e = 0;

    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < V - 1 && i < E) {
        Road nextRoad = roads[i++];

        if (i == exclude) continue;

        int x = find(nextRoad.src);
        int y = find(nextRoad.dest);

        if (x != y) {
            result[e++] = nextRoad;
            Union(x, y);
        }
    }
    if (e != V - 1) return 999999;

    int minimumCost = 0;
    for (i = 0; i < e; ++i)
        minimumCost += result[i].weight;
    return minimumCost;
}

int highwayPlanning() {
    int T; std::cin >> T;
    while (T-- > 0) {
        std::cin >> V >> E;
        for (int i = 0; i < E; ++i) {
            std::cin >> roads[i].src >> roads[i].dest >> roads[i].weight;
            --roads[i].src; --roads[i].dest;
        }

        std::qsort(roads, E, sizeof(Road), myComp);
        std::cout << KruskalMST() << " ";

        int secondBestCost = 999999, cost;
        for (int v = 0; v < V - 1; ++v) {
            cost = SecondKruskalMST(bestRoadsIdx[v]);
            if (secondBestCost > cost)
                secondBestCost = cost;
        }
        std::cout << secondBestCost << std::endl;
    }
    return 0;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(Calosseum2021, testHighwayPlanning) {
    std::streambuf* orig = std::cin.rdbuf();
    //std::istringstream input("1\n4 5\n1 2 1\n1 2 4\n2 3 2\n3 4 3\n3 4 5");  // 6 8
    //std::istringstream input("1\n6 10\n1 2 13\n1 3 28\n1 4 7\n2 3 27\n2 5 39\n3 4 2\n3 5 34\n3 6 14\n4 6 7\n5 6 36");  // 63 65
    std::istringstream input("2\n3 3\n1 2 1\n1 3 1\n2 3 1\n4 5\n1 2 5\n1 3 4\n2 3 2\n2 4 4\n3 4 5");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_EQ(highwayPlanning(), 0);

    std::cin.rdbuf(orig);
}