#include "prim.hpp"
#include "file_de_priorite.hpp" // Il y a les file dedans


std::vector<Arete*> get_voisins_ensemble(Graphe* g, std::vector<Sommet*> s, std::vector<bool>* deja_voisin) {
	bool est_voisin[g->get_nb_aretes()];

	std::vector<Arete*> res(0);

	for (int i = 0; i < g->get_nb_aretes(); ++i)
	{
		est_voisin[i] = false;
	}


	for (int i = 0; i < g->get_nb_aretes(); ++i)
	{

		if (!est_voisin[i] )
		{
			if ((*deja_voisin)[i])
			{
				res.push_back(g->get_arete(i));
				est_voisin[i] = true;

			} else {
				for (int j = 0; j < s.size(); ++j)
				{

					if (*s[j] == *g->get_sommet(g->get_arete(i)->parent) || *s[j] == *g->get_sommet(g->get_arete(i)->enfant))
					{

						res.push_back(g->get_arete(i));
						est_voisin[i] = true;
						(*deja_voisin)[i] = true;
						std::cout << g->get_arete(i)->enfant << "--" << g->get_arete(i)->parent << std::endl;

						break;
					}
				}
			}
			
		}
	}
	return res;
}


void prim(Graphe* g, Sommet* s, Graphe* acpd) {
	// Renvoie un arbre couvrant de poids minimal de g avec Prim
	// Hypothèse : g est non vide, s est le sommet du graphe depuis lequel on exécute prim
	// g doit être connexe
	// acpd est un pointeur vers un graphe, ce sera où on placera le résultat de prim
	// Au moins une arete dois partir de s
	acpd->zeroFlow();

	UnionFind ensemble_sommets(g->get_nb_sommets());

	std::vector<Sommet*> visites = {s};

	std::vector<bool> deja_voisin(g->get_nb_aretes(), false);

	while(visites.size() < g->get_nb_sommets()) {
		// vois <- On choisit les voisins direct de l'ensemble de sommets visites
		// On cherche le min des capacité parmi les voisins qui n'engendrent pas de cycle

		std::vector<Arete*> voisins = get_voisins_ensemble(g, visites, &deja_voisin);
		std::cout << "Taille voisins " << voisins.size() << std::endl;

		int f_min = -1;
		int i_min = -1;

		int classe_s_init = ensemble_sommets.find(s->aretes[0]->enfant);

		if (!(*g->get_sommet(s->aretes[0]->enfant) == *s))
		{
			std::cout << "parent\n" << std::endl;
			classe_s_init = ensemble_sommets.find(s->aretes[0]->parent);
		}



		for (int i = 0; i < voisins.size(); ++i)
		{
			if(ensemble_sommets.find(voisins[i]->enfant) != ensemble_sommets.find(voisins[i]->parent)) {
				if (voisins[i]->f < f_min || f_min == -1)
				{
					f_min = voisins[i]->f;
					i_min = i;
				}
			}
		}

		std::cout << voisins.size() << std::endl;
		assert(f_min != -1);

		std::cout << "arete cout min = " << f_min << std::endl;

		if (ensemble_sommets.find(voisins[i_min]->enfant) == classe_s_init)
		{
			visites.push_back(g->get_sommet(voisins[i_min]->parent));
		} else {
			visites.push_back(g->get_sommet(voisins[i_min]->enfant));
		}



		acpd->set_flow(voisins[i_min]->parent, voisins[i_min]->enfant, f_min);
		acpd->set_flow(voisins[i_min]->enfant, voisins[i_min]->parent, f_min);

		ensemble_sommets.join(ensemble_sommets.find(voisins[i_min]->parent), ensemble_sommets.find(voisins[i_min]->enfant));

	}

	for (int i = g->get_nb_aretes() - 1; i >= 0 ; --i)
	{
		if(acpd->get_arete(i)->f == 0) {
			std::cout << "Arete supprimée" << std::endl;
			acpd->del_arete(acpd->get_arete(i)->parent, acpd->get_arete(i)->enfant);
		}
	}
}


void emonder(Graphe* acpd) {
	// Supprime les arêtes en trop

	int n = acpd->get_nb_sommets();

	bool indicateur[n][n];

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			indicateur[i][j] = false;
		}
	}

	for (int k = 0; k < acpd->get_nb_aretes(); ++k)
	{
		int i = acpd->get_arete(k)->parent;
		int j = acpd->get_arete(k)->enfant;

		if (indicateur[i][j])
		{
			std::cout << "deux arêtes sont détectées et on les mêmes extrêmes" << std::endl;
		}
		indicateur[i][j] = true;
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if(indicateur[i][j] && indicateur[j][i]) {
				acpd->del_arete(i, j);
				indicateur[i][j] = false;
			}
		}
	}
}

void post_traitement(Graphe* acpd) {
	// Effectue le post traitement de acpd
	emonder(acpd);

	std::vector<int> deg(acpd->get_nb_sommets());

	for (int i = 0; i < acpd->get_nb_sommets(); ++i)
	{
		deg[i] = acpd->get_sommet(i)->aretes.size();
	}

	bool continuer = true;

	while(continuer) {
		bool changed = false;
		for (int i = 0; i < acpd->get_nb_sommets(); ++i)
		{
			if (deg[i] == 1 && acpd->get_sommet(i)->c == 0)
			{
				changed = true;
			
				int j = acpd->get_sommet(i)->aretes[0]->parent;
				if(j == i) j = acpd->get_sommet(i)->aretes[0]->enfant;
				acpd->del_arete(i, j);
				acpd->del_arete(j, i);

				deg[i]--;
				deg[j]--;
				std::cout << "Sommet supprimé" << std::endl;
			}
		}
		if (!changed)
		{
			continuer = false;
		}
	}

}

void prim_v(Graphe* g, Graphe* acpd) {
	// Exécute l'algorthme de Prim avec le vrai alogorithme

	acpd->zeroFlow();




	std::vector<float> couts(g->get_nb_sommets(), INFINITY);
	std::vector<int> peres(g->get_nb_sommets(), -1.f);

	emonder(acpd);

	FileDePriorite fp;



	for (int i = 0; i < g->get_nb_sommets(); ++i)
	{
		fp.enfile_prio(i, INFINITY);
			std::cout << acpd->get_nb_aretes() << "/ " << g->get_nb_aretes() << std::endl;

	}



	fp.reprio(0, 0.f);


	while(!fp.empty()) {

		fp.affiche();

		int t = fp.extract_min();


		for (int j = 0; j < g->get_nb_aretes(); j++)
		{
			Arete* arete = g->get_arete(j);


			if (arete->parent == t && couts[arete->enfant] >= arete->f && !fp.is_in(arete->enfant))
			{

				// On a une arete t--u
				int u = arete->enfant;

				peres[u] = t;
				couts[u] = arete->f;

				acpd->set_flow(t, u, arete->f);

				fp.reprio(u, couts[u]);



			} else if (arete->enfant == t && couts[arete->parent] >= arete->f && !fp.is_in(arete->parent)) {
				// On a une arete u--t
				int u = arete->parent;

				peres[u] = t;
				couts[u] = arete->f;

				acpd->set_flow(u, t, arete->f);

				fp.reprio(u, couts[u]);



			}



		}
	}

	print_vector(&peres);

	for (int i = acpd->get_nb_aretes() - 1; i >= 0 ; --i)
	{
			std::cout << acpd->get_nb_aretes() << "/ " << g->get_nb_aretes() << std::endl;
			std::cout << acpd->get_arete(i)->f << " = flot" << std::endl;

		if(acpd->get_arete(i)->f == 0) {	
			std::cout << "Arete supprimée" << std::endl;
			acpd->del_arete(acpd->get_arete(i)->parent, acpd->get_arete(i)->enfant);
		}
	}

	
}