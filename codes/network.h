#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <chrono>

#ifndef ROUTINGPROTOCOLS_NETWORK_H
#define ROUTINGPROTOCOLS_NETWORK_H
const int INF = 1e9;

struct Edge
{
    int dst;
    int cost;
    Edge(int dst_, int cost_);
};

std::vector<std::string> separating_words(std::string text, char disjunctive);

std::string pretty_string(int content, int desired_len);

class Network{

public:
    std::string initialize_topology(std::vector<std::string> string_edges);
    int get_edge_index(int src, int dst);
    int get_edge_index(std::vector<Edge> edges, int dst);
    int get_unmarked_min_edge_index(std::vector<Edge> edges, std::vector<bool> marks);
    std::string modify_edge(std::string string_edge);
    std::string remove_edge(std::string string_edge);
    void show();

private:
    std::map<int, std::vector<Edge>, std::less<int> > topology;
    int max_node;
};
#endif //ROUTINGPROTOCOLS_NETWORK_H
