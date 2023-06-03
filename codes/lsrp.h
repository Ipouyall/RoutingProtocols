#include "network.h"

#ifndef ROUTINGPROTOCOLS_LSRP_H
#define ROUTINGPROTOCOLS_LSRP_H

void print_lsrp_iter(int max_node, int iter_num, std::vector<Edge> nodes);
void lsrp(Network* network, int source, bool log_time=true);

#endif //ROUTINGPROTOCOLS_LSRP_H
