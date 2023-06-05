#include "network.h"
#include "lsrp.h"
#include "dvrp.h"

using namespace std;
using namespace std::chrono;

Edge::Edge(int dst_, int cost_) {
        dst = dst_;
        cost = cost_;
}

Network::Network(bool test) {
    test_mode = test;
}

std::vector<std::string> separating_words(std::string text, char disjunctive) {
    vector<string> words;
    stringstream line(text);
    while (line.good()) {
        string word;
        getline(line, word, disjunctive);
        words.push_back(word);
    }
    return words;
}

std::string pretty_string(int content, int desired_len){
    string res;
    int indent = desired_len - to_string(content).length();
    int pre_ = indent/2;
    for (int k = 0; k < indent - pre_; k++)
        res += " ";
    res += to_string(content);
    for (int k = 0; k < pre_; k++)
        res += " ";
    return res;
}

std::string Network::initialize_topology(std::vector<std::string> string_edges) {
    topology.clear();
    max_node = -1;
    for (string edge_string : string_edges) {
        vector<string> info = separating_words(edge_string, '-');
        if (info.size() != 3) {
            return "Error";
        }
        int node1 = stoi(info[0]);
        max_node = (max_node < node1) ? node1 : max_node;
        int node2 = stoi(info[1]);
        max_node = (max_node < node2) ? node2 : max_node;
        int cost = stoi(info[2]);
        if (node1 == node2) {
            return "Error";
        }
        auto node_src = topology.find(node1);
        if (node_src != topology.end()) {
            node_src->second.push_back(Edge(node2, cost));
        }
        else {
            vector<Edge> edges;
            edges.push_back(Edge(node2, cost));
            topology[node1] = edges;
        }
        auto node_dst = topology.find(node2);
        if (node_dst != topology.end()) {
            node_dst->second.push_back(Edge(node1, cost));
        }
        else {
            vector<Edge> edges;
            edges.push_back(Edge(node1, cost));
            topology[node2] = edges;
        }
    }
    return "OK";

}

int Network::get_edge_index(int src, int dst) {
    auto node = topology.find(src);
    for (int i = 0; i < node->second.size(); i++) {
        if (node->second[i].dst == dst)
            return i;
    }
    return -1;
}

int Network::get_edge_index(std::vector<Edge> edges, int dst) {
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].dst == dst)
            return i;
    }
    return -1;
}

int Network::get_unmarked_min_edge_index(std::vector<Edge> edges, std::vector<bool> marks) {
    int min = INF;
    int index = -1;
    for (int i = 0; i < edges.size(); i++) {
        if (!marks[i] && edges[i].cost < min) {
            min = edges[i].cost;
            index = i;
        }
    }
    return index;
}

string Network::modify_edge(string string_edge) {
    vector<string> info = separating_words(string_edge, '-');
    int node1 = stoi(info[0]);
    max_node = (max_node < node1) ? node1 : max_node;
    int node2 = stoi(info[1]);
    max_node = (max_node < node2) ? node2 : max_node;
    int cost = stoi(info[2]);
    if (node1 == node2) {
        return "Error";
    }
    auto node_src = topology.find(node1);
    if (node_src != topology.end()) {
        if (get_edge_index(node1, node2) != -1)
            node_src->second[get_edge_index(node1, node2)].cost = cost;
        else
            node_src->second.push_back(Edge(node2, cost));
    }
    else {
        vector<Edge> edges;
        edges.push_back(Edge(node2, cost));
        topology[node1] = edges;
    }
    auto node_dst = topology.find(node2);
    if (node_dst != topology.end()) {
        if (get_edge_index(node2, node1) != -1)
            node_dst->second[get_edge_index(node2, node1)].cost = cost;
        else
            node_dst->second.push_back(Edge(node1, cost));
    }
    else {
        vector<Edge> edges;
        edges.push_back(Edge(node1, cost));
        topology[node2] = edges;
    }
    return "OK";
}

std::string Network::remove_edge(std::string string_edge) {
    vector<string> info = separating_words(string_edge, '-');
    int node1 = stoi(info[0]);
    int node2 = stoi(info[1]);
    auto node_src = topology.find(node1);
    auto node_dst = topology.find(node2);
    if (node_src == topology.end() || node_dst == topology.end()) {
        return "Error";
    }
    if (get_edge_index(node1, node2) != -1)
        node_src->second.erase(node_src->second.begin() + get_edge_index(node1, node2));
    else
        return "Error";
    if (get_edge_index(node2, node1) != -1)
        node_dst->second.erase(node_dst->second.begin() + get_edge_index(node2, node1));
    else
        return "Error";
    return "OK";
}

void Network::show() {
    int section_size[max_node+1];
    section_size[0] = 1 + to_string(max_node).length();
    for (int i = 0; i < section_size[0]; i++)
        cout << ' ';
    cout << '|';
    for (int i = 1; i <= max_node; i++) {
        int max_sec_size = 0;
        for (int j = 1; j <= max_node; j++) {
            auto node = topology.find(i);
            int cost = i==j ? 0 : -1;
            if (node != topology.end() && get_edge_index(i, j) != -1)
                cost = node->second[get_edge_index(i, j)].cost;
            int sec_size = 2 + to_string(cost).length();
            max_sec_size = (max_sec_size < sec_size) ? sec_size : max_sec_size;
        }
        section_size[i] = max_sec_size;
    }
    for (int i = 1; i <= max_node; i++) {
        cout << pretty_string(i, section_size[i]) << '|';
    }
    cout << endl;
    for (int i = 0; i < section_size[0]; i++)
        cout << '-';
    cout << '+';
    for (int i = 1; i <= max_node; i++) {
        for (int j = 0; j < section_size[i]; j++)
            cout << '-';
        cout << '+';
    }
    cout << endl;
    for (int i = 1; i <= max_node; i++) {
        cout << i;
        int indent = section_size[0] - to_string(i).length();
        for (int j = 0; j < indent; j++)
            cout << ' ';
        cout << '|';
        for (int j = 1; j <= max_node; j++) {
            auto node = topology.find(i);
            int cost = i==j ? 0 : -1;
            if (node != topology.end() && get_edge_index(i, j) != -1)
                cost = node->second[get_edge_index(i, j)].cost;
            cout << pretty_string(cost, section_size[j]) << '|';
        }
        cout << endl;
    }
}

void Network::run_lsrp(int source) {
    lsrp(this, source, !test_mode);
}

void Network::run_lsrp_all() {
    cout << "***[Link State Routing Protocol]***" << endl;
    for (int i = 1; i <= max_node; i++) {
        cout << "***[Node " << i << "]***" << endl;
        run_lsrp(i);
        cout << '\n' << endl;
    }
}

void Network::run_dvrp(int source) {
        dvrp(this, source, !test_mode);
}

void Network::run_dvrp_all() {
    cout << "***[Distance Vector Routing Protocol]***" << endl;
    for (int i = 1; i <= max_node; i++) {
        cout << "***[Node " << i << "]***" << endl;
        run_dvrp(i);
        cout << '\n' << endl;
    }
}

map<int, vector<Edge>, less<int>> Network::get_topology() {
    return topology;
}

int Network::get_max_node() {
    return max_node;
}
