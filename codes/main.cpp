#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <chrono>
#include "network.h"

using namespace std;
using namespace std::chrono;

void run(Network& network, string cmd) {
    vector<string> cmd_parts = separating_words(cmd, ' ');
    if (cmd_parts[0] == "topology") {
        cmd_parts.erase(cmd_parts.begin());
        cout << network.initialize_topology(cmd_parts) << endl;
    }
    else if (cmd_parts[0] == "show") {
        network.show();
    }
    else if (cmd_parts[0] == "modify") {
        cout << network.modify_edge(cmd_parts[1]) << endl;
    }
    else if (cmd_parts[0] == "remove") {
        cout << network.remove_edge(cmd_parts[1]) << endl;
    }
    else if (cmd_parts[0] == "lsrp") {
        if (cmd_parts.size() == 2)
            network.run_lsrp(stoi(cmd_parts[1]));
        else
            network.run_lsrp_all();
    }
    else{
        cout << "Wrong command!" << endl;
    }
}

int main(int argc, char* argv[]) {
	cout << "welcome to network!" << endl;
    bool test_mode = false;
    if (argc == 2)
        if (string(argv[1]) == "test")
            test_mode = true;
    Network nn(test_mode);
    string cmd;
    while (getline(cin, cmd)) {
        run(nn, cmd);
    }
    //topology 1-2-19 1-3-9 2-4-3 1-4-5
	return 0;
}