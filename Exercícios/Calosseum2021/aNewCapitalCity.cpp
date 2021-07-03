#include <iostream>

#define INF 66666

static int numCities;
static int numRoads;
static char cities[25][11];
static int roads[25][25];

int aNewCapitalCity() {
    while (!std::cin.eof()) {
        std::cin >> numCities >> numRoads;
        for (int i = 0; i < numCities; ++i)
            std::cin >> cities[i];

        for (int i = 0; i < numCities; ++i) {
            for (int j = 0; j < numCities; ++j) {
                if (i == j) roads[i][j] = 0;
                else roads[i][j] = INF;
            }
        }

        int city1, city2, distance;
        for (int i = 0; i < numRoads; ++i) {
            std::cin >> city1 >> city2 >> distance;
            roads[city1 - 1][city2 - 1] = distance;
            roads[city2 - 1][city1 - 1] = distance;
        }

        int aux;
        for (int k = 0; k < numCities; ++k) {
            for (int i = 0; i < numCities; ++i) {
                for (int j = 0; j < numCities; ++j) {
                    aux = roads[i][k] + roads[k][j];
                    if (roads[i][j] > aux)
                        roads[i][j] = aux;
                }
            }
        }

        int bestCity = 0, minDistance = 99999;
        for (int i = 0; i < numCities; ++i) {
            distance = 0;
            for (int j = 0; j < numCities; ++j) {
                distance += roads[i][j];
            }
            if (minDistance > distance) {
                minDistance = distance;
                bestCity = i;
            }
        }

        std::cout << cities[bestCity] << std::endl;
    }
    return 0;
}
/*
#include <queue>
#include <list>

static int numCities;
static int numRoads;
static char cities[25][11];
static std::list<std::pair<int, int> > adj[25];
static int dist[25];
static bool visited[25];

int aNewCapitalCity() {
    while (!std::cin.eof()) {
        std::cin >> numCities >> numRoads;
        for (int i = 0; i < numCities; ++i) {
            std::cin >> cities[i];
            adj[i].clear();
        }

        int city1, city2, distance;
        for (int i = 0; i < numRoads; ++i) {
            std::cin >> city1 >> city2 >> distance;
            adj[city1 - 1].push_back(std::make_pair(city2 - 1, distance));
            adj[city2 - 1].push_back(std::make_pair(city1 - 1, distance));
        }

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> > , std::greater<std::pair<int, int> > > pq;

        int curSum, bestCity = 0, bestSum = INF;
        for (int city = 0; city < numCities; ++city) {
            pq.push(std::make_pair(0, city));

            for (int i = 0; i < numCities; ++i) {
                dist[i] = INF;
                visited[i] = false;
            }
            dist[city] = 0;

            int u, v, w;
            while (!pq.empty()) {
                u = pq.top().second;
                pq.pop();
                visited[u] = true;

                for (auto x : adj[u]) {
                    v = x.first;
                    w = x.second;

                    if (!visited[v] && dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        pq.push(std::make_pair(dist[v], v));
                    }
                }
            }
            curSum = 0;
            for (int i = 0; i < numCities; ++i)
                curSum += dist[i];
            if (bestSum > curSum) {
                bestSum = curSum;
                bestCity = city;
            }
        }
        std::cout << cities[bestCity] << std::endl;
    }
    return 0;
}*/

/// TESTS ///
#include <gtest/gtest.h>

TEST(Calosseum2021, testANewCapitalCity) {
    std::streambuf* orig = std::cin.rdbuf();
    std::istringstream input("2 1\nOlisippo\nCale\n1 2 308\n3 3\nBogota\nLeticia\nTarapaca\n1 2 1092\n1 3 963\n2 3 148");
    std::cin.rdbuf(input.rdbuf());

    EXPECT_EQ(aNewCapitalCity(), 0);

    std::cin.rdbuf(orig);
}