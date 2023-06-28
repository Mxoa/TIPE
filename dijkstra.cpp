#include "dijkstra.hpp"
#include "file_de_priorite.hpp"



int find_min(std::list<int> ouverts, std::vector<float>* dist) {
	// Trouve la plus petit indice de ouverts
	if (ouverts.empty())
	{
		std::cout << "Erreur ?" << std::endl;
		return -1;
	}

	float dist_min = (*dist)[ouverts.front()];
	int sommet_min = ouverts.front();

	for(auto it = std::begin(ouverts); it != std::end(ouverts); it++)
	{
		if ((*dist)[*it] < dist_min)
		{
			dist_min = (*dist)[*it];
			sommet_min = *it;
		}
	}

	return sommet_min;
}

std::vector<int> dijkstra(Graphe_m* g, int s) {
	// Algo de dijkstra qui renvoi un tableau de père dans g pour arriver en s
	// les sommets sont des entiers de 0 à n - 1
	// Rnevoie les sommets pères 

	int nb_sommets = g->get_nb_sommets();

	std::vector<float> dist(nb_sommets, INFINITY);
	std::vector<int> pere(nb_sommets, -1);

	//std::list<int> ouverts;
	FileDePriorite ouverts;

	//ouverts.push_front(s);
	ouverts.enfile_prio(s, 0);

	dist[s] = 0;
	pere[s] = s;

	while(!ouverts.empty()) {
		// int u = find_min(ouverts, &dist);
		int u = ouverts.extract_min();

		std::list<paire> succs = g->get_successeurs(u);


		for (auto it = std::begin(succs); it != std::end(succs); it++)
		{
			int v = it->first;

			if (dist[v] == INFINITY) {
				// ouverts.push_front(v);
				ouverts.enfile_prio(v, dist[v]);
			}
			if(dist[u] + g->get_cout(u, v) < dist[v]) {
				dist[v] = dist[u] + g->get_cout(u, v);
				pere[v] = u;
			}
		}
		
		

		// ouverts.remove(u);
		//ouverts.remove_min();


		// print_list(&ouverts);
		//ouverts.affiche();


	}

	return pere;
}

doublet_dist_chem dijkstra_d_p(Graphe_m* g, int s) {
	// Algo de dijkstra qui renvoi un tableau de père dans g pour arriver en s
	// les sommets sont des entiers de 0 à n - 1
	// Rnevoie les sommets pères 

	int nb_sommets = g->get_nb_sommets();

	std::vector<float> dist(nb_sommets, INFINITY);
	std::vector<int> pere(nb_sommets, -1);

	//std::list<int> ouverts;
	FileDePriorite ouverts;

	//ouverts.push_front(s);
	ouverts.enfile_prio(s, 0);

	dist[s] = 0;
	pere[s] = s;

	while(!ouverts.empty()) {
		// int u = find_min(ouverts, &dist);
		int u = ouverts.extract_min();

		std::list<paire> succs = g->get_successeurs(u);

		for (auto it = std::begin(succs); it != std::end(succs); it++)
		{
			int v = it->first;
			if (dist[v] == INFINITY) {
				// ouverts.push_front(v);
				ouverts.enfile_prio(v, dist[v]);
			}
			if(dist[u] + g->get_cout(u, v) < dist[v]) {
				dist[v] = dist[u] + g->get_cout(u, v);
				pere[v] = u;
			}
		}
		
		

		// ouverts.remove(u);
		//ouverts.remove_min();

		//std::cout << u << ")\n" << std::endl;

		// print_list(&ouverts);
		//ouverts.affiche();


	}

	return std::make_pair(dist, pere);
}