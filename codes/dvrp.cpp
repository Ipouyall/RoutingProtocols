#include "dvrp.h"

using namespace std;
using namespace std::chrono;


void print_dvrp(vector<vector<int>> paths, vector<int> dist, int source, int max_node) {
    //cout << "Dest | Next Hop | Dist | Shortest-Path" << endl;
    cout << "Dest";
    for (int i = 0; i < to_string(max_node).length(); i++)
        cout << " ";
    cout << "|";
    cout << " Next Hop";
    for (int i = 0; i < to_string(max_node).length(); i++)
        cout << " ";
    cout << "|";
    cout << " Dist";
    for (int i = 0; i < to_string(*max_element(dist.begin(), dist.end())).length(); i++)
        cout << " ";
    cout << " Shortest-Path";
    int max_len = -1;
    for (int i = 0; i < paths.size(); i++) {
        int temp = 0;
        for (int j = 0; j < paths[i].size(); j++) {
            temp += to_string(paths[i][j]).length() + 2;
        }
        if (temp > max_len)
            max_len = temp;
    }
    cout << endl;
    max_len += 4 + 1 * to_string(max_node).length() + to_string(*max_element(dist.begin(), dist.end())).length() + 1 + 9 + 1 + 5 + 14;
    while (max_len--)
        cout << "-";
    cout << "-" << endl;

    for (int i = 1; i <= max_node; i++) {
        cout << i ;
        for (int j = 0; j < to_string(max_node).length() - to_string(i).length() + 4; j++)
            cout << " ";
        cout << "| ";
        if (i != source && dist[i - 1] != 0) {
            cout << paths[i - 1][1];
            for (int j = 1; j < to_string(max_node).length() - to_string(paths[i - 1][1]).length() + 9; j++)
                cout << " ";
        }
        else if (dist[i - 1] == 0 && i != source) {
            cout << paths[i - 1][1];
            for (int j = 1; j < to_string(max_node).length() - to_string(-1).length() + 9; j++)
                cout << " ";
        }
        else {
            cout << i;
            for (int j = 1; j < to_string(max_node).length() - to_string(i).length() + 9; j++)
                cout << " ";
        }
        cout << "| ";
        


        cout << dist[i - 1];
        for (int j = 0; j < to_string(*max_element(dist.begin(), dist.end())).length() - to_string(dist[i - 1]).length() + 5; j++)
            cout << " ";
        cout << "| ";

        cout << "[";
        for (int j = 0; j < paths[i - 1].size(); j++) {
            if (j != (int)paths[i - 1].size() - 1)
                cout << paths[i - 1][j] << "->";
            else
                cout << paths[i - 1][j];

        }
        cout << "]" << endl;
    }

}

void dvrp(Network* network, int source, bool log_time) {
    int max_node = network->get_max_node();
    if (source > max_node) {
        cout << "Error invalid source number" << endl;
        return;
    }
    map<int, vector<Edge>, less<int>> topology = network->get_topology();
    map<int, vector<Edge>, less<int>> tables;
    for (int i = 0; i < max_node; i++) {
        vector<Edge> temp;
        tables.insert({ i + 1, temp });
    }
    vector<vector<int>> paths(max_node);
    vector<int> current_nodes;
    vector<int> next_nodes;
    vector<int> fathers_current(max_node, -1);
    vector<int> fathers_next(max_node, -1);
    vector<int> dist(max_node);

    auto node = topology.find(source);
    dist[source - 1] = 0;

    for (int i = 0; i < node->second.size(); i++) {
        tables.find(source)->second.push_back(Edge(node->second[i].dst, node->second[i].cost));
        next_nodes.push_back(node->second[i].dst);
        fathers_next[node->second[i].dst - 1] = source;
    }
    auto start = std::chrono::steady_clock::now();
    auto stop = std::chrono::steady_clock::now();
    bool updated = true;
    paths[source - 1].push_back(source);
    tables.find(source)->second.push_back(Edge(source, 0));
    while (updated) {
        current_nodes = next_nodes;
        fathers_current = fathers_next;
        for (int i = 0; i < fathers_next.size(); i++)
            fathers_next[i] = -1;
        next_nodes.clear();
        updated = false;
        for (int i = 0; i < current_nodes.size(); i++) {
            auto main_node = topology.find(current_nodes[i]);
            auto table = tables.find(current_nodes[i]);
            int father_num = fathers_current[current_nodes[i] - 1];
            vector<Edge> father_table = tables.find(father_num)->second;
            vector<int> path = paths[father_num - 1];
            for (int j = 0; j < father_table.size(); j++) {
                if (father_table[j].dst == current_nodes[i]) {
                    int index = network->get_edge_index(table->second, father_table[j].dst);
                    int current_index_in_father = network->get_edge_index(father_table, current_nodes[i]);
                    if (index == -1) {
                        table->second.push_back(Edge(current_nodes[i], father_table[current_index_in_father].cost));
                        updated = true;
                        fathers_next[current_nodes[i] - 1] = father_num;
                        dist[father_table[j].dst - 1] = father_table[j].cost;
                        path.push_back(current_nodes[i]);
                        paths[current_nodes[i] - 1] = path;
                    }
                    else if (table->second[index].cost > father_table[j].cost) {
                        updated = true;
                        table->second[index].cost = father_table[j].cost;
                        fathers_next[current_nodes[i] - 1] = father_num;
                        dist[father_table[j].dst - 1] = father_table[current_index_in_father].cost;
                        path.push_back(current_nodes[i]);
                        paths[current_nodes[i] - 1] = path;
                    }
                }
                else {
                    int index = network->get_edge_index(table->second, father_table[j].dst);
                    int current_index_in_father = network->get_edge_index(father_table, current_nodes[i]);
                    if (index == -1) {
                        updated = true;
                        table->second.push_back(Edge(father_table[j].dst, father_table[j].cost + father_table[current_index_in_father].cost));
                    }
                    else if (table->second[index].cost > father_table[j].cost + father_table[current_index_in_father].cost) {
                        updated = true;
                        table->second[index].cost = father_table[j].cost + father_table[current_index_in_father].cost;
                    }
                }
            }
            for (int j = 0; j < main_node->second.size(); j++) {
                int index = network->get_edge_index(table->second, main_node->second[j].dst);
                int current_index_in_father = network->get_edge_index(father_table, current_nodes[i]);
                if (index == -1) {
                    updated = true;
                    table->second.push_back(Edge(main_node->second[j].dst, father_table[current_index_in_father].cost + main_node->second[j].cost));
                    fathers_next[main_node->second[j].dst - 1] = current_nodes[i];
                    next_nodes.push_back(main_node->second[j].dst);
                }
                else if (table->second[index].cost > main_node->second[j].cost + father_table[current_index_in_father].cost) {
                    updated = true;
                    table->second[index].cost = main_node->second[j].cost + father_table[current_index_in_father].cost;
                    fathers_next[main_node->second[j].dst - 1] = current_nodes[i];
                    next_nodes.push_back(main_node->second[j].dst);
                }
            }
            tables.find(current_nodes[i])->second = table->second;
        }
    }
    stop = std::chrono::steady_clock::now();
    print_dvrp(paths, dist, source, max_node);
    auto duration = duration_cast<nanoseconds>(stop - start);
    if (log_time)
        cout << "time: " << duration.count() << " ns" << endl;
}