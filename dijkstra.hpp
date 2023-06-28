#ifndef DIKJSTRA_HPP
#define DIKJSTRA_HPP

#include "graphe_m.hpp"
#include "file_de_priorite.hpp"

typedef std::pair<std::vector<float>, std::vector<int>> doublet_dist_chem;

int find_min(std::list<int> ouverts, std::vector<float>* dist);

std::vector<int> dijkstra(Graphe_m* g, int s);

doublet_dist_chem dijkstra_d_p(Graphe_m* g, int s);




#endif