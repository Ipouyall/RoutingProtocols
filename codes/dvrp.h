#include "network.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include<string>
#include <chrono>
#include <algorithm>

#ifndef ROUTINGPROTOCOLS_DVRP_H
#define ROUTINGPROTOCOLS_DVRP_H

void print_dvrp(std::vector<std::vector<int>> paths, std::vector<int> dist, int source, int max_node);
void dvrp(Network* network, int source, bool log_time=true);

#endif //ROUTINGPROTOCOLS_DVRP_H