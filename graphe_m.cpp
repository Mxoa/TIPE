#include "graphe_m.hpp"

Graphe_m::Graphe_m(int nb_sommets) : m_voisins(nb_sommets) {
	// Créer un Graphe_m à nb_sommets sommets [0, nb_sommets-1]
}

void Graphe_m::add_edge(unsigned int u, unsigned int v, float c) {
	// Ajoute l'arête (u, v) au Graphe_m avec un coût c
	assert(u < m_voisins.size() && v < m_voisins.size() && u >= 0 && v >= 0);
	m_voisins[u].push_front(std::make_pair(v, c));
}

std::list<paire> Graphe_m::get_successeurs(int i) {
	// Renvoie une copie de la liste des voisins du sommet i
	return m_voisins[i];
}

Graphe_m to_graphe_m(Graphe* g, bool capacity_as_w) {
	Graphe_m res(g->get_nb_sommets());

	auto ar = g->get_aretes();

	for (Arete* arete : ar)
	{
		if (capacity_as_w)
		{
			res.add_edge(arete->parent, arete->enfant, arete->c);
		} else {
			res.add_edge(arete->parent, arete->enfant, arete->f);
		}
	}

	return res;
}

float Graphe_m::get_cout(unsigned int u, unsigned int v) {
	// Retourne le coût de l'arête (u, v)
	auto it = std::begin(m_voisins[u]);
	while(it->first != v && it != std::end(m_voisins[u])) {
		it++;
	}
	assert(it->first == v && "L'arête n'existe pas.");

	return it->second;
}

void Graphe_m::affiche() const {
	// Affiche le Graphe_m
	for (unsigned int i = 0; i < m_voisins.size(); ++i)
	{
		std::cout << i << " | ";
		for (auto it = std::begin(m_voisins[i]); it != std::end(m_voisins[i]); ++it)
		{
			std::cout << "(" << it->first << ", c = " << it->second << ") ";
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

int Graphe_m::get_nb_sommets() const {
	return m_voisins.size();
}



void Graphe_m::supprime_chemin(chemin* ch) {
	//supprime le chemin ch du Graphe_m (seulement les arêtes)
	//hyp : le chemin est bien un chemin du Graphe_m au sens où toutes les arêtes sont dedans
	assert(contient_chemin(ch));
	
	
	for (auto it = std::begin(*ch); it != std::end(*ch); ++it)
	{
		auto next = it;
		next++;
		if (next != std::end(*ch))
		{
			m_voisins[*it].remove(std::make_pair(*next, get_cout(*it, *next)));
		}
	}
}

bool Graphe_m::contient_chemin(chemin* ch) {
	// Vérifie que ch appartienne bien au Graphe_m
	bool appartient = true;
	for (auto it = std::begin(*ch); it != std::end(*ch); ++it)
	{
		auto next = it;
		next++;
		if (next != std::end(*ch))
		{
			bool est_dans_vois = false;
			for(auto itt = std::begin(m_voisins[*it]); itt != std::end(m_voisins[*it]); ++itt) {
				if ((unsigned int)*next == itt->first)
				{
					est_dans_vois = true;
				}
			}
			if (!est_dans_vois)
			{
				appartient = false;
			}
		}
	}
	return appartient;
}


Graphe Graphe_m::to_graphe(std::vector<Sommet*> sommets) {
	// Convertit le graphe en graphe affichable
	// Cette fonction génère un graphe avec comme sommets des sommets prédéfinis

	assert(sommets.size() == m_voisins.size() && "il n'y a pas le même nombre de sommets");

	Graphe res(sommets);

	// Il y a donc sommets.size() sommets à l'intérieur

	for (unsigned int i = 0; i < m_voisins.size(); ++i)
	{
		for (auto it = std::begin(m_voisins[i]); it != std::end(m_voisins[i]); ++it)
		{
			res.add_arete(i, it->first, it->second);
		}
	}

	return res;

}
