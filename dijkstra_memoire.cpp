#include "dijkstra_memoire.hpp"

std::ostream& operator<<(std::ostream& flux, floatf const& z) {
	if (z.inf)
	{
		flux << "+inf";
	} else {
		flux << z.val;
	}
	return flux;
}

bool operator==(floatf const& x, floatf const& y) {
	if (x.inf && y.inf)
	{
		return true;
	} else if (x.inf && ! y.inf) {
		return false;
	} else if (!x.inf && y.inf) {
		return false;
	} else {
		return x.val == y.val;
	}
}

bool operator<(floatf const& x, floatf const& y) {
	if (x.inf && y.inf)
	{
		return false;
	} else if (x.inf && ! y.inf) {
		return false;
	} else if (!x.inf && y.inf) {
		return true;
	} else {
		return x.val < y.val;
	}
}

bool operator<=(floatf const& x, floatf const& y) {
	return x < y || x == y;
}

bool operator>=(floatf const& x, floatf const& y) {
	return !(x < y);
}

bool operator>(floatf const& x, floatf const& y) {
	return !(x <= y);
}

floatf operator+(floatf const& x, float const& y) {
	if (x.inf)
	{
		return x;
	}
	floatf w;
	w.inf = false;
	w.val = x.val + y;
	return w;
}

floatf operator+(floatf const& x, floatf const& y) {
	if (x.inf && y.inf)
	{
		return x;
	}
	return x + y.val;
}

floatf to_floatf(float x) {
	floatf w;
	w.inf = false;
	w.val = x;
	return w;
}

float to_float(floatf x) {
	assert(!x.inf);
	return x.val;
}



Dijkstra_memoire::Dijkstra_memoire(Graphe_m* g, ensemble_points* ville, ensemble_points* loti) :
	m_distance_min (g->get_nb_sommets()),
	m_enfant_prefere(g->get_nb_sommets()),
	m_peres(g->get_nb_sommets()),
	m_graphe (g),
	m_ville (ville),
	m_loti (loti)
{
	for (int i = 0; i < g->get_nb_sommets(); ++i)
	{
		m_distance_min[i].inf = true;
	}
}

Dijkstra_memoire::~Dijkstra_memoire() {

}

void Dijkstra_memoire::affiche_distance() const {
	std::cout << "[";
	for (unsigned int i = 0; i < m_distance_min.size(); ++i)
	{
		std::cout << m_distance_min[i] << ", ";
	}
	std::cout << "]";
	std::cout << std::endl;
}


chemin Dijkstra_memoire::trouve_chemin(bool verbose) {
	// Trouve le chemin de distance minimale qui relie la ville au lotissement en se servant de mémoire
	auto_resize();

	floatf infty;
	infty.inf = true;
	std::vector<int> appartenance(m_graphe->get_nb_sommets(), 0); // Pour permettre de tester si un sommet est dans la ville ou dans le lotissement en O(1)
	for (auto it = std::begin(*m_ville); it != std::end(*m_ville); ++it)
	{
		appartenance[*it] = 1;
	}


	for (auto it = std::begin(*m_loti); it != std::end(*m_loti); ++it)
	{
		appartenance[*it] = -1;
	}

	if(verbose) std::cout << "Tableau d'appartenance généré." << std::endl;

	
	bool chem_au_moins_un = false;

	for (auto it = std::begin(*m_ville); it != std::end(*m_ville); ++it)
	{
		int sommet = *it;
		if(verbose) std::cout << "	Traitement du sommet " << sommet << std::endl;

		FileDePriorite fp;
		std::vector<floatf> dista(m_graphe->get_nb_sommets(), infty);
		std::vector<int> peres(m_graphe->get_nb_sommets(), -1);

		int sommet_final = -1;

		fp.enfile_prio(sommet, 0.);

		floatf zero;
		zero.inf = false;
		zero.val = 0;

		dista[sommet] = zero;
		peres[sommet] = sommet;
		bool dans_loti = false;

		while(!fp.empty() && !dans_loti) {

			int s = fp.extract_min();
			if(verbose) std::cout << "		Le min de la file est " << s << std::endl;




			if (appartenance[s] == -1) {
				// Si on aboutit dans le lotissement, on stoppe l'algo, on remontera le chemin avec sommet_final
				sommet_final = s;
				dans_loti = true;
				if(verbose) std::cout << "Nous sommes arrivés dans le lotissement" << std::endl;
			}else if (!m_distance_min[s].inf) {
				// Si un chemin existe depuis s jusqu'au lotissement...
				int u = s;
				// On accélère et on suit les enfants préférés jusqu'au lotissement
				while(appartenance[u] != -1) {
					peres[m_enfant_prefere[u]] = u;
					dista[u] = m_distance_min[u];
					u = m_enfant_prefere[u];
				}
				dista[u] = zero;
				sommet_final = u;
				dans_loti = true;
				if(verbose) std::cout << "Nous accélérons" << std::endl;
			} else {
				std::list<paire> successeurs = m_graphe->get_successeurs(s);
				if(verbose) affiche_distance();
				if(verbose) print_vector<int>(&peres);
				for (auto itt = std::begin(successeurs); itt != std::end(successeurs); itt++) {
					int succ = itt->first;
					float cout = itt->second;

					if (dista[succ].inf)
					{
						fp.enfile_prio(succ, to_float(dista[s] + to_floatf(cout)));
					}
					if (dista[s] + to_floatf(cout) < dista[succ])
					{
						dista[succ] = dista[s] + cout;
						peres[succ] = s;
					}
					
				}
			}
		}
		// On remonte le chemin pour mettre à jour le chemin des enfants préférés
		// En particulier on retourne forcément jusqu'à 'sommet'
		if(verbose) std::cout << "Fin Dijkstra - Remonter les sommets à partir de " << sommet_final << std::endl;

		if (sommet_final != -1) {
			int u = sommet_final;
			if(verbose) print_vector<int>(&peres);
			if(verbose) print_vector<floatf>(&dista);
			while(peres[u] != u) {
				assert(u != -1);
				if(verbose) std::cout << "Sommet : " << u << std::endl;
				m_enfant_prefere[peres[u]] = u;
				m_distance_min[u] = dista[u];
				if(verbose) std::cout << "dista[u] = " << dista[u] << std::endl;
				m_peres[u] = peres[u];
				u = peres[u];
			}
			if(verbose) std::cout << u << std::endl;
			if(verbose) affiche_distance();
			if(verbose) print_vector<int>(&peres);
			if(verbose) std::cout << "Fin Dijkstra" << std::endl;
			chem_au_moins_un = true;
		}
	}

	if (chem_au_moins_un)
	{
		floatf dmin;
		dmin.inf = true;

		int sommet_min = 0;

		for (auto it = std::begin(*m_loti); it != std::end(*m_loti); ++it)
		{
			// On cherche ici le sommet du lotissement qui minimise la distance à la ville
			int sommet = *it;
			if(verbose) std::cout << "Distance minimale = " << m_distance_min[sommet] << std::endl;
			if(m_distance_min[sommet] < dmin) {
				sommet_min = sommet;
			}
		}

		// sommet_min est le sommet duquel il faut partir (du lotissement)	
		chemin ch;
		int u = sommet_min;
		while(appartenance[u] != 1) {
			ch.push_front(u);
			u = m_peres[u];
		}

		ch.push_front(u);


		return ch;
	} else {
		if(verbose) std::cout << "chemin inexistant" << std::endl;
		chemin ch(0);
		return ch;
	}

	
}

void Dijkstra_memoire::auto_resize() {
	m_peres.resize(m_graphe->get_nb_sommets());
	m_enfant_prefere.resize(m_graphe->get_nb_sommets());
	m_distance_min.resize(m_graphe->get_nb_sommets());

	for (int i = 0; i < m_graphe->get_nb_sommets(); ++i)
	{
		m_enfant_prefere[i] = -1;
		m_peres[i] = -1;
		m_distance_min[i].inf = true;
	}
}