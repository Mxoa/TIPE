#include "recherche.hpp"

std::vector<chemin> recherche(Graphe_m *g, std::vector<int>* points_ville, std::vector<int>* points_loti, int k, bool verbose) {
	// Hyp points_loti et points_ville sont disjoints
	Graphe_m copie_g(*g);

	if(verbose) std::cout << "Le Graphe_m a été copié" << std::endl;

	std::vector<chemin> res(k, std::list<int>(0));

	for (int j = 0; j < k; ++j)
	{
		if(verbose) std::cout << "Recherche du chemin n°" << j + 1 << std::endl;

		// distance minimale de raccordement lotissement-ville
		float distance_minimale = INFINITY;


		for (unsigned int i = 0; i < points_ville->size(); ++i)
		{
			if(verbose) std::cout << "	On analyse le sommet ville i = " << i << "/" << points_ville->size() - 1 << " (" << (*points_ville)[i] << ")" << std::endl;

			doublet_dist_chem dblet = dijkstra_d_p(&copie_g, (*points_ville)[i]);
			int u = find_indice_min_in_vect(&dblet.first, points_loti);

			if(verbose) std::cout << "		Le point de raccord minimal dans le lotissement est le n° = " << u << " (" << (*points_loti)[u] << ")" << std::endl;

			if(verbose) {std::cout << "		Distances : "; print_vector<float>(&dblet.first);}
			if(verbose) {std::cout << "		Pères : "; print_vector<int>(&dblet.second);}

			if (dblet.first[u] == INFINITY)
			{
				if(verbose) std::cout << "		Impossible de raccorder lotissement-Ville en fin de compte (distance infinie)" << std::endl;

				// Si jamais il n'y a plus assez de sommets on s'arrête
				break;
			}

			if (dblet.first[u] < distance_minimale)
			{
				// Reconstitution du chemin à partir du tableau de pères
				if(verbose) std::cout << "		Une distance plus petite a été trouvée : nv dist min = " << dblet.first[u] << std::endl;

				chemin ch;
				int w = u;
				do {
					std::cout << w << " " << std::endl;
					ch.push_front(w);
					w = (dblet.second)[w];
				} while(w != (dblet.second)[w]);

				res[j] = ch;
				res[j].push_front((*points_ville)[i]);
				distance_minimale = dblet.first[u];
			}
		}

		if(verbose) std::cout << "Suppression du chemin 'minimal'" << std::endl;
		if(verbose) copie_g.affiche();
		if(verbose) {std::cout << "Chemin à supprimer : "; print_list<int>(&res[j]);}
		copie_g.supprime_chemin(&res[j]);
		if(verbose) copie_g.affiche();
		if(verbose) std::cout << "______________________________________" << std::endl;
	}

	return res;
}

std::vector<chemin> recherche_memoire_avec_flot_suffisant(Graphe *g, int conso_loti, std::vector<int>* points_ville, std::vector<int>* points_loti, int k, bool verbose) {

	std::vector<int> points_ville_cpy = *points_ville;
	std::vector<int> points_loti_cpy = *points_loti;

	Graphe_m g_m = to_graphe_m(g, true);

	Sommet virtuel(0, 0, 0);

	std::vector<Arete> arrs(0);

	std::vector<int> pts_raccord(0);

	std::vector<chemin> res;

	virtuel.x = -2;

	virtuel.y = -2;

	virtuel.c = conso_loti;

	bool ajout_p = false;


	do {

		arrs.clear();

		pts_raccord.clear();

		res = recherche_memoire(&g_m, &points_ville_cpy, &points_loti_cpy, k, verbose);


		for (unsigned int i = 0; i < res.size(); ++i)
		{
			if (!res[i].empty())
			{
				pts_raccord.push_back(res[i].front());

				std::cout << "Point de raccord envisagé : " << pts_raccord[i] << std::endl;

			}
		}

		

		for (unsigned int i = 0; i < pts_raccord.size(); ++i)
		{
			Arete arr;
			arr.parent = pts_raccord[i];
			arr.enfant = -1;
			arr.f = 0;
			arr.c = conso_loti;
			arrs.push_back(arr);
		}

		if (pts_raccord.size() > 0)
		{

			std::vector<int> tmp;

			for (unsigned int i = 0; i < points_ville_cpy.size(); ++i)
			{
				if (pts_raccord[0] != points_ville_cpy[i])
				{
					tmp.push_back(points_ville_cpy[i]);
				}
			}

			points_ville_cpy = tmp;

		}

		if (ajout_possible(g, virtuel, arrs, false))
		{
			ajout_p = true;
		}


	} while(!ajout_p && !points_ville_cpy.empty() && pts_raccord.size() > 0);

	if(ajout_p) {
		ajout_possible(g, virtuel, arrs, true);
		std::cout << "Ajout possible" << std::endl;
		demande_satisfiable(g, true);

		//g->del_sommet(g->get_nb_sommets() - 1);
	}
	
	return res;	
}


std::vector<chemin> resolve(Graphe** ville_cpyy, Graphe* ville, Graphe* route, Graphe* loti, std::vector<Arete>& ville_route, std::vector<Arete>& route_loti, int k, bool verbose) {
	
	Graphe* loti_cpy = loti->copy();
	Graphe* ville_cpy = ville->copy();
	Graphe* route_cpy = route->copy();

	for (int i = 0; i < loti_cpy->get_nb_sommets(); ++i)
	{
		loti_cpy->get_sommet(i)->c = 0;
	}

	loti_cpy->zeroFlow();
	ville_cpy->zeroFlow();
	route_cpy->zeroFlow();

	std::vector<int> pts_loti(0);
	std::vector<int> pts_ville(0);

	// Connexion des graphes... sur ville_cpy, on détermine pts ville et pts loti en mm temps

	int k_0 = ville_cpy->get_nb_sommets();

	ville_cpy->add_graphe(loti_cpy);

	int k_1 = ville_cpy->get_nb_sommets();

	ville_cpy->add_graphe(route_cpy);

	for (int i = 0; i < k_0; ++i)
	{
		pts_ville.push_back(i);
	}

	for (int i = k_0; i < k_1; ++i)
	{
		pts_loti.push_back(i);
	}

	for (unsigned int i = 0; i < ville_route.size(); ++i)
	{
		ville_cpy->add_arete(ville_route[i].parent, k_1 + ville_route[i].enfant, ville_route[i].c);
	}

	for (unsigned int i = 0; i < route_loti.size(); ++i)
	{
		ville_cpy->add_arete(k_1+route_loti[i].parent, k_0 + route_loti[i].enfant, route_loti[i].c);
	}

	// Les poids de la routes sont sur les capacités

	std::vector<chemin> res = recherche_memoire_avec_flot_suffisant(ville_cpy, loti->get_consommation(), &pts_ville, &pts_loti, k);

	// Recopier les arêtes dans la ville
	for (int i = 0; i < ville->get_nb_sommets(); ++i)
	{
		for (unsigned int j = 0; j < ville->get_sommet(i)->aretes.size(); ++j)
		{
			int parent = ville->get_sommet(i)->aretes[j]->parent;
			int enfant = ville->get_sommet(i)->aretes[j]->enfant;
			ville->set_flow(parent, enfant, ville_cpy->get_flow(ville_cpy->get_sommet(i)->aretes[j]->parent, ville_cpy->get_sommet(i)->aretes[j]->enfant));
		}
	}


	for (int i = 0; i < ville->get_nb_sommets(); i++) {
		Sommet* sommet = ville_cpy->get_sommet(i);

		int j = 0;

		for (Arete* a : sommet->aretes) {
			if (a->parent == i) {
				ville->set_flow(a->parent, a->enfant, ville_cpy->get_flow(a->parent, a->enfant));
				j++;
			}
		}
	}

	*ville_cpyy = ville_cpy->copy();

	delete loti_cpy;
	delete route_cpy;
	delete ville_cpy;

	return res;
}

std::vector<chemin> recherche_memoire(Graphe_m *g, std::vector<int>* points_ville, std::vector<int>* points_loti, int k, bool verbose) {
	// Hyp points_loti et points_ville sont disjoints
	Graphe_m copie_g(*g);
	std::vector<chemin> ch_v(0);

	Dijkstra_memoire d_m(&copie_g, points_ville, points_loti);

	for (int i = 0; i < k; ++i)
	{
		chemin ch = d_m.trouve_chemin(verbose);
		std::cout << "Chemin trouvé :";
		print_list<int>(&ch);
		//list_rev<int>(&ch);
		//print_list<int>(&ch);
		ch_v.push_back(ch);
		copie_g.supprime_chemin(&ch_v[i]);
	}

	return ch_v;
}



int find_indice_min_in_vect(std::vector<float>* dist, std::vector<int>* indices_autorises) {
	int indice_min = (*indices_autorises)[0];
	float dist_associee = (*dist)[indice_min];

	for (unsigned int i = 0; i < indices_autorises->size(); ++i)
	{
		if (dist_associee > (*dist)[(*indices_autorises)[i]])
		{
			dist_associee = (*dist)[(*indices_autorises)[i]];
			indice_min = (*indices_autorises)[i];
		}
	}

	return indice_min;
}

int rd(int min, int max){
    // Hyp : srand(time(0)) dans main
    return rand()%(max-min) + min;
}




chemin creer_chemin_aleatoire(Graphe_m* g, std::vector<int>* ville,std::vector<int>* lotissement) {
	// Génere un chemin aléatoire reliant la ville au lotissement dans G
	int i = rd(0, ville->size());
	int j = rd(0, lotissement->size());
	std::vector<int> pt_ville = {(*ville)[i]};
	std::vector<int> pt_loti = {(*lotissement)[j]};
	auto dp = recherche(g, &pt_loti, &pt_ville, 1);
	if (dp.size() == 0)
	{
		assert("Pas réussi à créer un chemin aléatoire.");
	}
	return dp[0];
}
