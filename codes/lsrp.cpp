#include "lsrp.h"

using namespace std;
using namespace std::chrono;

void print_lsrp_iter(int max_node, int iter_num, vector<Edge> nodes) {
    cout << "Iter " << iter_num << ":" << endl;
    int indent = 5 + to_string(iter_num).length();
    int indents[max_node+1], max_len = indent;
    indents[0] = indent + 1;
    cout << "Dest";
    for (int i = 0; i < indent-4; i++)
        cout << " ";
    cout << "|";
    for (int i=0; i<nodes.size(); i++) {
        indents[i + 1] = 2 + max(to_string(nodes[i].dst).length(), to_string(nodes[i].cost).length());
        max_len +=  indents[i + 1]+1;
    }
    for (int i = 1; i <= max_node; i++)
        cout << pretty_string(i, indents[i]) << "|";
    cout << endl;
    cout << "cost";
    for (int i = 0; i < indent-4; i++)
        cout << " ";
    cout << "|";
    for (int i=0; i<nodes.size(); i++)
        cout << pretty_string(nodes[i].cost, indents[i+1]) << "|";
    cout << endl;

    while (max_len--)
        cout << "-";
    cout << "-" << endl;
}


void lsrp(Network* network, int source, bool log_time) {
    auto start = std::chrono::steady_clock::now();
    auto topology = network->get_topology();
    int max_node = network->get_max_node();
    vector<Edge> nodes;
    vector<bool> marks(max_node, false);
    vector<vector<int> > paths(max_node);
    auto node = topology.find(source);
    auto stop = start;
    auto print_time = std::chrono::nanoseconds (0);

    for (int i = 1; i <= max_node; i++) {
        if (i == source) {
            nodes.push_back(Edge(i, 0));
            continue;
        }
            nodes.push_back(Edge(i, INF));
    }
    paths[source - 1].push_back(source);

    for (int i = 0; i < max_node; i++) {
        int min_index = network->get_unmarked_min_edge_index(nodes, marks);
        if (min_index == -1)
            break;
        int mark_node_num = min_index + 1;
        auto mark_node = topology.find(mark_node_num);
        marks[min_index] = true;

        if (i != 0) {
            auto print_st = std::chrono::steady_clock::now();
            print_lsrp_iter(max_node, i, nodes);
            auto print_ed = std::chrono::steady_clock::now();
            print_time += std::chrono::duration_cast<std::chrono::nanoseconds>(print_ed - print_st);
        }

        for (int j = 0; j < mark_node->second.size(); j++) {
            if (nodes[min_index].cost + mark_node->second[j].cost >= nodes[mark_node->second[j].dst - 1].cost)
                continue;
            nodes[mark_node->second[j].dst - 1].cost = nodes[min_index].cost + mark_node->second[j].cost;
            vector<int> parent_path = paths[min_index];
            parent_path.push_back(mark_node->second[j].dst);
            paths[mark_node->second[j].dst - 1] = parent_path;
        }

    }
    stop = std::chrono::steady_clock::now();

    cout << "Path [s] -> [d] | Min-Cost | Shortest-Path" << endl;
    for (int i = 0; i < nodes.size(); i++) {
        if (i + 1 == source)
            continue;
        cout << source << "->" << i + 1 << " | " << ((nodes[i].cost == INF) ? -1 : nodes[i].cost) << " | ";
        for (int j = 0; j < (int)paths[i].size(); j++) {
            if (j != (int)paths[i].size() - 1)
                cout << paths[i][j] << "->";
            else
                cout << paths[i][j];
        }
        cout << endl;
    }

    auto duration = duration_cast<nanoseconds>(stop - start - print_time);
    if (log_time)
        cout << "time: " << duration.count() << " ns" << endl;
}
