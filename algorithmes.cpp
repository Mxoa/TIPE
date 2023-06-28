#include "algorithmes.h"

#include <algorithm>
#include <queue>
#include <cmath>



// Renvoie un chemin améliorant entre s et p dans g. Renvoie un chemin vide si aucun chemin améliorant n'existe.
// Hyp : s et p sont des sommets de g.
Chemin chemin_ameliorant(Graphe* g, int s, int p) {

	// Initialise la liste des parents et des sommets visités.
	int* parents = new int[g->get_nb_sommets()];
	bool* visited = new bool[g->get_nb_sommets()];
	for (int i = 0; i < g->get_nb_sommets(); i++) {
		parents[i] = -1;
		visited[i] = false;
	}

	// Effectue un parcourt en largeur en partant de s.
	std::queue<int> q;
	q.push(s);
	int current = -1;
	while(!q.empty() && current != p) {
		current = q.front();
		q.pop();
		visited[current] = true;

		// Ajoute les sommets non visités accessibles depuis current à la file. 
		for (Arete* a : g->get_sommet(current)->aretes) {
			if (a->parent == current && !visited[a->enfant]) {

				parents[a->enfant] = current;
				q.push(a->enfant);

			}
		}

	}

	// Construit et renvoie le chemin.
	Chemin c;
	if (current == p) {
		while (current != -1) {
			c.push_back(current);
			current = parents[current];
		}
		std::reverse(c.begin(), c.end());
	}
	delete [] parents;
	delete [] visited;
	return c;

} 

// Applique l'algorithme de Ford-Fulkerson entre s et p sur la graphe g.
// Hyp : s et p sont des sommets de g.
void ford_fulkerson(Graphe* g, int s, int p, sf::RenderWindow* window, sf::Font* font) {
	g->zeroFlow();
	int i = 0;

	// Enregistre une image du graphe initial.
	if (window != nullptr && font != nullptr) {
		g->screenshot(*window, *font, std::to_string(i), true);
		i++;
	}

	// Tant qu'on peut trouver un chemin améliorant.
	Graphe* r = g->gen_residual_graph();
	Chemin c = chemin_ameliorant(r, s, p);
	while (c.size() != 0) {

		// Calcule la modification de flot maximale pouvant être faite sur le chemin.
		float min = r->get_capacity(c[0], c[1]);
		for (int i = 0; i < c.size()-1; i++) {

			float cap = r->get_flow(c[i], c[i+1]);
			if (min > cap) min = cap;

		}	

		// Applique la modification.
		for (int i = 0; i < c.size()-1; i++) {

			if (g->get_capacity(c[i], c[i+1]) != 0) 
				 g->set_flow(c[i], c[i+1], g->get_flow(c[i], c[i+1])+min);
			else g->set_flow(c[i], c[i+1], g->get_flow(c[i], c[i+1])-min);
			
		}

		// Calcule le chemin suivant en prévision du prochain test de boucle.
		delete r;
		r = g->gen_residual_graph();
		c = chemin_ameliorant(r, s, p);

		// Enregistre une image de l'étape.
		if (window != nullptr && font != nullptr) {
			g->screenshot(*window, *font, std::to_string(i), true);
			i++;
		}

	}
	delete r;

}



// Vérifie si le réseau peut satisfaire toutes les demandes. 
// Hyp : Il n'y a qu'une source.
bool demande_satisfiable(Graphe* g, bool edit, sf::RenderWindow* window, sf::Font* font) {

	// Récupère la liste des puits et la source.
	// Crée une liste contenant une copie des sommets plus un puit virtuel.
	int source;
	int sum = 0;
	int x = 0, y = 0;
	std::vector<int> puits;
	std::vector<Sommet*> sommets;
	for (int i = 0; i < g->get_nb_sommets(); i++) {
		Sommet* s = g->get_sommet(i);
		
		if (x < s->x) x = s->x;
		if (y < s->y) y = s->y;
		if (s->c < 0) source = i;
		if (s->c > 0) {
			puits.push_back(i);
			sum += s->c;
		}
		sommets.push_back(new Sommet(s->x, s->y, std::min(s->c, 0)));

	}
	sommets.push_back(new Sommet(x+1,y+1, sum));
	int v = sommets.size()-1;

	// Construit une copie du graphe et y ajoute un unique puit virtuel.
	Graphe* g2 = new Graphe(sommets);
	for (int i = 0; i < g->get_nb_sommets(); i++) {
		for (Arete* a : g->get_sommet(i)->aretes) {
			if (a->parent == i) g2->add_arete(a->parent, a->enfant, a->c, a->f);
		}
	}
	for (int i : puits) g2->add_arete(i, v, g->get_sommet(i)->c);


	// Applique Ford-Fulkerson sur le graphe construit.
	ford_fulkerson(g2, source, v, window, font);

		if (window != nullptr && font != nullptr) {
			window->clear();
			g2->draw(*window, *font, false);
			window->display();
			sf::Texture texture;
			texture.create(window->getSize().x, window->getSize().y);
			texture.update(*window);
			texture.copyToImage().saveToFile("screenshot/BLYAT.jpg");
		}

	// Vérifie si tout les puits reçoivent bien assez de flot.
	bool ok = true;
	for (int i : puits) {
		if (g2->get_capacity(i,v) != 0 && g2->get_flow(i,v) != g->get_sommet(i)->c) ok = false;
	}

	// Modifie si besoin le flot du graphe originel.
	if (ok) {
		for (int i = 0; i < v; i++) {
			for (Arete* a : g2->get_sommet(i)->aretes) {
				if (a->enfant != v && a->parent == i) g->set_flow(a->parent, a->enfant, g2->get_flow(a->parent, a->enfant));
			}
		}
	}

	// Renvoie le résultat.
	delete g2;
	return ok;

}

// Vérifie si le réseau peut supporter l'ajout d'un nouveau sommet et ses arêtes.
// Hyp : s n'est pas un sommet de g et les éléments de aretes sont des arètes entre un sommet de g et -1 représentant s.
bool ajout_possible(Graphe* g, Sommet s, std::vector<Arete>& aretes, bool edit, sf::RenderWindow* window, sf::Font* font) {
	
	// Crée une copie du graphe et ajoute le nouveau sommet et ses arètes.
	Graphe* graphe = g->copy();
	graphe->add_sommet(s.x, s.y, 0);
	int sommet = graphe->get_nb_sommets()-1;
	for (int i = 0; i < aretes.size(); i++) {
		if (aretes[i].parent == -1) aretes[i].parent = sommet;
		else aretes[i].enfant = sommet;
		graphe->add_arete(aretes[i].parent, aretes[i].enfant, aretes[i].c, 0);
	}

	// Vérifie l'intégrité du nouveau réseau et renvoie le résultat en modifiant le graphe initial si besoin.
	bool res = demande_satisfiable(graphe, true, window, font);

	// Modifie au besoin le graphe initial.
	if (res && edit) {
		g->add_sommet(s.x, s.y, s.c);
		for (Arete a : aretes) {
			g->add_arete(a.parent, a.enfant, a.c);
		}
		for (int i = 0; i < sommet; i++) {
			for (Arete* a : g->get_sommet(i)->aretes) {
				if (a->parent == i) g->set_flow(a->parent, a->enfant, g->get_flow(a->parent, a->enfant)); 
			}
		}
	}

	delete graphe;
	return res;

}

// Renvoie true si l'ajout des chemins de chemins permet d'assurer un approvisionnement de tout le réseau. 
// hyp : les chemins de chemin commencent par un sommet de capacité -1 dans routes.
//		 tout les sommets de routes sont dans g.
bool ajout_chemin_possible(Graphe* g, Graphe* routes, std::vector<chemin>* chemins, bool edit, sf::RenderWindow* window, sf::Font* font) {

	// Crée une copie du graphe et ajoute les sommets intermédiaires.
	Graphe* graphe = g->copy(); 
	int inter = -1;
	for (int i = 0; i < routes->get_nb_sommets(); i++) {

		// Récupère les sommets intermédiaires de routes et les ajoutent au graphe.
		Sommet* s = routes->get_sommet(i);
		if (s->c == 0) {
			if (inter == -1) inter = i;
			graphe->add_sommet(s->x, s->y, 0);
		}

	}

	// Ajoute les chemins au graphe.
	int conso = (int)(10.0f * std::ceil((float)routes->get_consommation()/10.0f));
	for (int i = 0; i < chemins->size(); i++) {
		if ((*chemins)[i].size() > 1) {

			// Parcourt un chemin.
			std::vector<int> chem((*chemins)[i].size());
		    std::copy((*chemins)[i].begin(), (*chemins)[i].end(), chem.begin());
			for (int j = 0; j < chem.size() - 1; j++) {

				// Récupère les indices de chaques sommets des chemins de routes dans g.
				int s1 = -1;
				int s2 = -1;
				Sommet* route_s1 = routes->get_sommet(chem[j]);
				Sommet* route_s2 = routes->get_sommet(chem[j+1]);
				for (int k = 0; k < graphe->get_nb_sommets(); k++) {

					Sommet* graphe_s = graphe->get_sommet(k);
					if (*route_s1 == *graphe_s) s1 = k;
					if (*route_s2 == *graphe_s) s2 = k;

				}

				// Ajoute l'arète.
				graphe->add_arete(s1, s2, conso);

			}
		}

	}

	// Supprime les sommets innutilisés.
	for (int i = graphe->get_nb_sommets() - 1; i >= g->get_nb_sommets(); i--) {
		if (graphe->get_sommet(i)->aretes.size() == 0) graphe->del_sommet(i);
	}

	// Vérifie la satisfiabilité de la demande et fait les modifications sur la graphe initial si besoin.
	bool res = demande_satisfiable(graphe, edit, window, font);
	if (res && edit) {

		// Supprime les arètes inutiles entre le graphe de la ville et du quartier.
		for (int i = graphe->get_nb_aretes() - 1; i >= g->get_nb_aretes(); i--) {
			if (graphe->get_arete(i)->f == 0) graphe->del_arete(i);
		}
		
		// Supprime les sommets inutiles entre le graphe de la ville et du quartier.
		for (int i = graphe->get_nb_sommets() - 1; i >= 0; i--) {
			if (graphe->get_sommet(i)->aretes.size() == 0) graphe->del_sommet(i);
		}

		// Effectue la copie.
		g->copy_from(graphe);

	}

	// Renvoie le resultat.
	delete graphe;
	return res;

}



// Renvoie true si les segments [a1,a2] et [b1, b2] ont un point d'intersection.
bool intersection(Sommet* a1, Sommet* a2, Sommet* b1, Sommet* b2) {

	// Initialise des vecteurs.
	int v1[2] = {a1->x-b1->x, a1->y-b1->y};
	int v3[2] = {b1->x-a1->x, b1->y-a1->y};
	int v4[2] = {b2->x-a1->x, b2->y-a1->y};
	int v2[2] = {a2->x-b1->x, a2->y-b1->y};
	int va[2] = {a2->x-a1->x, a2->y-a1->y};
	int vb[2] = {b2->x-b1->x, b2->y-b1->y};

	// Calcule leurs déterminants.
	int det1 = v1[0]*vb[1]-vb[0]*v1[1];
	int det2 = v2[0]*vb[1]-vb[0]*v2[1];
	int det3 = v3[0]*va[1]-va[0]*v3[1];
	int det4 = v4[0]*va[1]-va[0]*v4[1];

	// En déduit l'existence d'un point d'intersection. 
	return ((det1 <= 0 && det2 >= 0) || (det1 >= 0 && det2 <= 0)) &&
		   ((det3 <= 0 && det4 >= 0) || (det3 >= 0 && det4 <= 0));

}

// Fonction de calcul des sommets directement accessibles depuis un point.
std::vector<int> sommets_accessibles(Graphe* g, Sommet s) {

	// Pour chaque sommet, parcourt les arètes.
	std::vector<int> sommets;
	for (int i = 0; i < g->get_nb_sommets(); i++) {

		bool res = false;
		for (int j = 0; j < g->get_nb_aretes(); j++) {

			// Vérifie qu'aucune arète du graphe ne contenant pas i ne coupe le segment [s,i].
			Arete* a = g->get_arete(j);
			if (a->parent != i && a->enfant != i) {
				res |= intersection(
					g->get_sommet(a->parent), g->get_sommet(a->enfant),
					g->get_sommet(i), &s 
				);
			}

		}
		if (!res && g->get_sommet(i)->c == 0) sommets.push_back(i);

	}

	return sommets;

}