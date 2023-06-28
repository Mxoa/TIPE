#include "quartier.h"

#include "algorithmes.h"

#include <iostream>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <utility>
#include <ctime>
#include <cmath>



// Génère un quartier aléatoirement avec des consommation
Graphe* generer_quartier(int nb_pts_route, int nb_maison, int dispertion, int x_0, int y_0, int conso_moyenne, int dispertion_conso) {

	srand(time(0));
	Graphe res({});

	int i = 0;
	while(i < nb_pts_route) {
		int x = rand() % (2*dispertion) - dispertion + x_0;
		int y = rand() % (2*dispertion) - dispertion + y_0;

		bool casser = false;
		for (int j = 0; j < res.get_nb_sommets(); ++j) {
			if (*res.get_sommet(j) == Sommet(x, y, 0)) casser = true;
		}

		if(!casser) {
			
			res.add_sommet(x, y, 0);
			std::vector<int> sommets_connectables = sommets_accessibles(&res, *res.get_sommet(i));
			for (int j = 0; j < sommets_connectables.size(); ++j) {

				int w = res.get_sommet(sommets_connectables[j])->x;
				int h = res.get_sommet(sommets_connectables[j])->y;
				res.add_arete(i, sommets_connectables[j], (h-y)*(h-y) + (w-x)*(w-x), (h-y)*(h-y) + (w-x)*(w-x));
			
			}
			i++;

		}

	}

	i = 0;
	std::vector<Sommet*> maisons;
	std::vector<int> route;
	while(i < nb_maison) {

		int x = rand() % (2*dispertion) - dispertion + x_0;
		int y = rand() % (2*dispertion) - dispertion + y_0;
		int conso = std::max(rand() % (2*dispertion_conso) - dispertion_conso + conso_moyenne, 1);

		bool casser = false;
		for (int j = 0; j < res.get_nb_sommets(); ++j) {
			if (*res.get_sommet(j) == Sommet(x, y, 0)) casser = true;
		}

		for (int j = 0; j < maisons.size(); ++j) {
			if (Sommet(x, y, 0) == *maisons[j]) casser = true;
		}

		if(!casser) {
			maisons.push_back(new Sommet(x, y, conso));
			i++;
		}

	}

	for (int i = 0; i < nb_maison; ++i) {
		
		std::vector<int> sommets_connectables = sommets_accessibles(&res, *maisons[i]);
		int dist_min = 10*dispertion;
		int sommet_dist_min = 0;
		int j = 0;

		do {

			int w = res.get_sommet(sommets_connectables[j])->x;
			int h = res.get_sommet(sommets_connectables[j])->y;
			int x = maisons[i]->x;
			int y = maisons[i]->y;

			if ((w-x)*(w-x)+(h-y)*(h-y) < dist_min) {
				dist_min = (w-x)*(w-x)+(h-y)*(h-y);
				sommet_dist_min = sommets_connectables[j];
			}

			j++;

		} while(j < sommets_connectables.size());
		route.push_back(sommet_dist_min);

	}

	for (int i = 0; i < maisons.size(); ++i) {
		res.add_sommet(maisons[i]);
		res.add_arete(route[i], res.get_nb_sommets() - 1, 1, 1);
	}

	// Assure que la capacité est suffisante pour le flot nécessaire.
	int conso = res.get_consommation();
	for (int i = 0; i < res.get_nb_aretes(); i++) {
		res.get_arete(i)->c = (int)(10.0f * std::ceil((float)conso/10.0f));
	}
	for (int i = 0; i < res.get_nb_sommets(); i++) {
		Sommet* s = res.get_sommet(i);
		if (s->c > 0) for (Arete* a : s->aretes) a->c = (int)(10.0f * std::ceil((float)s->c/10.0f));
	}

	return res.copy();

}



// Génère le graphe des routes.
Graphe* generer_routes(Graphe* ville, Graphe* quartier, int nb_sommets_inter, int dispertion) {
	srand(time(nullptr));

	// Récupère les sommets de route dans le quartier.
	std::vector<int> sommets_route_quartier;
	for (int i = 0; i < quartier->get_nb_sommets(); i++) {
		if (quartier->get_sommet(i)->c == 0) sommets_route_quartier.push_back(i);
	}

	// Récupère les sommets accessibles depuis le quartier.
	std::vector<int> sommets_acc_ville;
	for (int s : sommets_route_quartier) {

		// Réucupère les sommets accessibles depuis un sommet route du quartier.
		std::vector<int> sommets = sommets_accessibles(ville, *quartier->get_sommet(s));
		for (int i = 0; i < sommets.size(); i++) {
			if (!std::count(sommets_acc_ville.begin(), sommets_acc_ville.end(), sommets[i]))
				sommets_acc_ville.push_back(sommets[i]);
		}

	}

	// Génère le nouveau graphe.
	Graphe* routes = quartier->copy();
	for (int s : sommets_acc_ville) {
		Sommet* sommet = ville->get_sommet(s);
		routes->add_sommet(sommet->x, sommet->y, 0);
	}

	// Calcule le barycentre des sommets accessibles.
	int x_acc = 0;
	int y_acc = 0;
	for (int s : sommets_acc_ville) {
		x_acc += ville->get_sommet(s)->x;
		y_acc += ville->get_sommet(s)->y;
	}
	x_acc /= sommets_acc_ville.size();
	y_acc /= sommets_acc_ville.size();

	// Calcule le barycentre des sommets de route du quartier.
	int x_q = 0;
	int y_q = 0;
	for (int s : sommets_route_quartier) {
		x_q += quartier->get_sommet(s)->x;
		y_q += quartier->get_sommet(s)->y;
	}
	x_q /= sommets_route_quartier.size();
	y_q /= sommets_route_quartier.size();

	// Calcule le barycentre des points intermédiaires et le vecteur directeur de
	// la droite qui les porte.
	int x_route = (x_acc + x_q) / 2;
	int y_route = (y_acc + y_q) / 2;
	int x_dir = x_route - x_acc;
	int y_dir = y_acc - y_route;
	int d = sqrt(x_dir*x_dir + y_dir*y_dir);
	x_dir *= dispertion;
	y_dir *= dispertion;
	x_dir /= d;
	y_dir /= d;

	// Ajout des sommets intermédiaires.
	for (int i = 0; i < nb_sommets_inter; i++) {
		routes->add_sommet(
			x_route + (nb_sommets_inter/2-i)*x_dir + rand()%(dispertion/2)-(dispertion/4), 
			y_route + (nb_sommets_inter/2-i)*y_dir + rand()%(dispertion/2)-(dispertion/4),
			0
		);
	}

	// Relie ces sommets au points du quartier.
	for (int i = 0; i < nb_sommets_inter; i++) {

		int s_i = routes->get_nb_sommets()-nb_sommets_inter+i;
		std::vector<int> sommets = sommets_accessibles(routes, *routes->get_sommet(s_i));
		for (int s2_i : sommets) {
			if (s2_i < quartier->get_nb_sommets()) {
				Sommet* s = routes->get_sommet(s_i);
				Sommet* s2 = routes->get_sommet(s2_i);
				routes->add_arete(s_i, s2_i, 0, std::sqrt((float)(s->x-s2->x)*(s->x-s2->x) + (float)(s->y-s2->y)*(s->y-s2->y)));
			}
		}

	}

	// Trie les sommets accessibles dans la ville.
	std::vector<std::pair<int,int> > paires;
	for (int i = 0; i < sommets_acc_ville.size(); i++) {

		int s = i + quartier->get_nb_sommets();
		Sommet* sommet = routes->get_sommet(s);
		paires.push_back(std::pair<int,int>(s, sommet->x*x_dir + sommet->y*y_dir));

	}
	std::sort(paires.begin(), paires.end(), [](auto &left, auto &right) {
		return left.second < right.second;
	});

	// Construit les arètes vers la ville.
	int q = paires.size()/nb_sommets_inter;
	for (int i = 0 ; i < paires.size(); i++) {
		Sommet* s = routes->get_sommet(paires[i].first);
		Sommet* s2 = routes->get_sommet(routes->get_nb_sommets() - std::min(nb_sommets_inter, i/q+1));
		routes->add_arete(
			paires[i].first,
			routes->get_nb_sommets() - std::min(nb_sommets_inter, i/q+1),
			0,
			std::sqrt((float)(s->x-s2->x)*(s->x-s2->x) + (float)(s->y-s2->y)*(s->y-s2->y))
		);
	}

	// Met les consommation des points de la ville à -1;
	for (int i = 0; i < paires.size(); i++) {
		routes->get_sommet(paires[i].first)->c = -1;
	}
	
	// Met les consommation des points de orute du quartier à -2;
	for (int i = 0; i < quartier->get_nb_sommets(); i++) {
		Sommet* s = routes->get_sommet(i);
		if (s->c == 0) s->c = -2;
	}
	
	return routes;

}
