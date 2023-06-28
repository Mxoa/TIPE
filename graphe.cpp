#include "graphe.h"

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>

#include "drawing.h"



// Opérateurs sur les sommets.
bool operator==(const Sommet& u, const Sommet& v) {
	return u.x == v.x && u.y == v.y;
}

std::ostream& operator<<(std::ostream& out, Sommet const& s) {
	out << "{" << s.x << ", " << s.y << "}";
	return out;
}

// Opérateurs sur les arètes.
bool operator==(const Arete& u, const Arete& v) {
	return u.parent == v.parent && u.enfant == v.enfant;
}



// Constructeur.
Graphe::Graphe(std::vector<Sommet*> sommets) {

	// Vérifie que chaque sommet est unique et enregistre la liste des sommets.
	for (int i = 0; i < sommets.size(); ++i) {
		for (int j = 0; j < sommets.size(); ++j) {
			if (j != i && (*(sommets[i]) == *(sommets[j]))) {
				throw std::runtime_error("Un sommet apparaît en double dans le graphe");
			}
		}
	}
	this->sommets = sommets;

	// Recalcule la taille de la zone d'affichage.
	gen_rect();

}

// Destructeur.
Graphe::~Graphe() {

	// Libère les sommets.
	for (Sommet* s : sommets) {
		delete s;
	}

	// Libère les arètes.
	for (Arete* a : aretes) {
		delete a;
	}

}

// Fonction d'ajout d'une arète.
void Graphe::add_arete(int s1, int s2, int c, int f) {

	// Crée l'arète.
	Arete* a = new Arete();
	a->parent = s1;
	a->enfant = s2;
	a->c = c;
	a->f = f;

	// Ajoute l'arète à la liste et aux sommets.
	aretes.push_back(a);
	sommets[s1]->aretes.push_back(a);
	sommets[s2]->aretes.push_back(a);

}

std::vector<Arete*> Graphe::get_aretes() {
	return aretes;
}

// Ajoute un deuxième graphe à celui-ci, on suppose qu'il n'y a pas de sommet en commun
void Graphe::add_graphe(Graphe* other) {

	int k_0 = this->get_nb_sommets();

	for (int i = 0; i < other->sommets.size(); ++i)
	{
		this->add_sommet(other->sommets[i]->x, other->sommets[i]->y, other->sommets[i]->c);
	}

	for (int i = 0; i < other->aretes.size(); ++i)
	{
		assert(other->aretes[i]->parent + k_0 < this->get_nb_sommets() && other->aretes[i]->enfant + k_0 < this->get_nb_sommets());
		this->add_arete(other->aretes[i]->parent + k_0, other->aretes[i]->enfant + k_0, other->aretes[i]->c, other->aretes[i]->f);
	}
}

// Supprime une arète.
void Graphe::del_arete(int s1, int s2) {

	// Récupère l'arète entre s1 (parent) et s2 (enfant).
	Arete* a = nullptr;
	Sommet* sommet_1 = get_sommet(s1);
	Sommet* sommet_2 = get_sommet(s2);
	for (int i = 0; i < sommet_1->aretes.size(); i++) {
		if (sommet_1->aretes[i]->parent == s1 && sommet_1->aretes[i]->enfant == s2) {
			a = sommet_1->aretes[i];
			break;
		}
	}

	// Si elle existe, la supprime.
	if (a != nullptr) {

		std::vector<Arete*>::iterator pos = std::find(sommet_1->aretes.begin(), sommet_1->aretes.end(), a);
		if (pos != sommet_1->aretes.end()) sommet_1->aretes.erase(pos);

		pos = std::find(sommet_2->aretes.begin(), sommet_2->aretes.end(), a);
		if (pos != sommet_2->aretes.end()) sommet_2->aretes.erase(pos);

		pos = std::find(aretes.begin(), aretes.end(), a);
		if (pos != aretes.end()) aretes.erase(pos);

		delete a;

	}

}

// Supprime une arète.
void Graphe::del_arete(int id) {

	if (id >= 0 && id < aretes.size()) {

		Arete* a = aretes[id];
		Sommet* sommet_1 = get_sommet(a->parent);
		Sommet* sommet_2 = get_sommet(a->enfant);

		std::vector<Arete*>::iterator pos = std::find(sommet_1->aretes.begin(), sommet_1->aretes.end(), a);
		if (pos != sommet_1->aretes.end()) sommet_1->aretes.erase(pos);

		pos = std::find(sommet_2->aretes.begin(), sommet_2->aretes.end(), a);
		if (pos != sommet_2->aretes.end()) sommet_2->aretes.erase(pos);

		pos = std::find(aretes.begin(), aretes.end(), a);
		if (pos != aretes.end()) aretes.erase(pos);

		delete a;

	}

}

// Ajoute un sommet dans le graphe.
void Graphe::add_sommet(int x, int y, int c) {
	Sommet* s = new Sommet(x,y,c);

	bool present = false;
	for (int i = 0; i < get_nb_sommets(); i++) {
		if (*s == *get_sommet(i)) present = true;
	}

	if (present) delete s;
	else sommets.push_back(s);

	// Recalcule la taille de la zone d'affichage.
	gen_rect();

}

void Graphe::add_sommet(Sommet* sommet) {

	bool present = false;
	for (int i = 0; i < get_nb_sommets(); i++) {
		if (*sommet == *get_sommet(i)) present = true;
	}

	if (present) delete sommet;
	else sommets.push_back(sommet);

	// Recalcule la taille de la zone d'affichage.
	gen_rect();
}


// Supprime un sommet du graphe.
void Graphe::del_sommet(int s) {

	// Si le sommet est dans le graphe, parcourt ses arètes.
	if (s >= 0 && s < get_nb_sommets()) {
		Sommet* sommet = get_sommet(s);
		for (Arete* a : sommet->aretes) {

			// Récupère un itérateur pointant sur l'adresse de l'arète dans le sommet voisin et dans la liste des sommets. 
			Sommet* sommet_2 = get_sommet((s == a->parent) ? a->enfant : a->parent);
			auto it1 = std::find(sommet_2->aretes.begin(), sommet_2->aretes.end(), a);
			auto it2 = std::find(aretes.begin(), aretes.end(), a);

			// Supprime l'adresse dans les listes et supprime l'arète.
			if (it1 != sommet_2->aretes.end()) sommet_2->aretes.erase(it1);
			if (it2 != aretes.end()) aretes.erase(it2);
			delete a;

		}

		// Supprime le sommet et supprime son adresse de la liste des sommets.
		sommets.erase(std::find(sommets.begin(), sommets.end(), sommet));
		delete sommet;

		// Renumérote les sommets dans les arètes.
		for (Arete* a : aretes) {
			if (a->parent > s) a->parent--;
			if (a->enfant > s) a->enfant--;
		}

	}

	// Recalcule la taille de la zone d'affichage.
	gen_rect();

}

// Met à zéro les flots du graphe.
void Graphe::zeroFlow() {
	for (Arete* a : aretes) a->f = 0;
}

// Renvoie un sommet selon son identifiant dans la liste.
Sommet* Graphe::get_sommet(unsigned int id) {
	
	if (id >= 0 && id < sommets.size()) return sommets[id];
	return nullptr;

}

// Renvoie le nombre de sommets.
int Graphe::get_nb_sommets() {
	return sommets.size();
}

// Renvoie la capacitée d'une arète dans le graphe.
int Graphe::get_capacity(int s1, int s2) {

	for (Arete* a : get_sommet(s1)->aretes) {
		if (a->enfant == s2) return a->c;
	}
	return 0;

}

// Renvoie le flot d'une arète dans le graphe.
int Graphe::get_flow(int s1, int s2) {

	for (Arete* a : get_sommet(s1)->aretes) {
		if (a->enfant == s2) return a->f;
	}
	return 0;

}

// Renvoie le flot d'une arète dans le graphe.
int Graphe::get_flow(int a) {

	if (a >= 0 && a < aretes.size()) return aretes[a]->f;
	return 0;

}

// Renvoie une copie du graphe.
Graphe* Graphe::copy() {

	// Crée une copie des sommets.
	std::vector<Sommet*> sommets;
		for (int i = 0; i < get_nb_sommets(); i++) {
		Sommet* sommet = get_sommet(i);
		sommets.push_back(new Sommet(sommet->x, sommet->y, sommet->c));
	}

	// Crée une copie du graphe et ajoute les arêtes.
	Graphe* graphe = new Graphe(sommets);
	for (int i = 0; i < get_nb_sommets(); i++) {
		Sommet* sommet = get_sommet(i);
		for (Arete* a : sommet->aretes) {
			if (a->parent == i) graphe->add_arete(a->parent, a->enfant, a->c, a->f);
		}
	}

	return graphe;

}

// Renvoie le nombre d'arêtes dans le graphe
int Graphe::get_nb_aretes() {
	return aretes.size();
}

// Renvoie l'arête n°id
Arete* Graphe::get_arete(unsigned int id) {

	if (id >= 0 && id < aretes.size()) return aretes[id];
	return nullptr;

}



// Renvoie le graphe résiduel associé.
Graphe* Graphe::gen_residual_graph() {

	// Crée une copie de chaque sommet du graphe et l'ajoute à une liste.
	std::vector<Sommet*> sommets;
	for (int i = 0; i < get_nb_sommets(); i++) {		

		Sommet* s1 = get_sommet(i);
		Sommet* s2 = new Sommet(s1->x, s1->y, s1->c);
		sommets.push_back(s2);

	}

	// Crée le graphe résiduel et initialise ses arètes.
	Graphe* r = new Graphe(sommets);
	for (Arete* a : aretes) {

		if (a->c-a->f != 0) r->add_arete(a->parent, a->enfant, a->c, a->c-a->f);
		if (a->f != 0) r->add_arete(a->enfant, a->parent, a->c, a->f);

	}

	return r;

}

int Graphe::get_consommation() {
	int somme = 0;
	for (Sommet* sommet : sommets)
	{
		somme += std::max(sommet->c, 0);
	}
	return somme;
}

// Copie un graphe.
void Graphe::copy_from(Graphe* g) {

	// Vide le graphe.
	sommets.clear();
	for (Arete* a : aretes) delete a;
	aretes.clear();

	// Copie les sommets.
	for (int i = 0; i < g->get_nb_sommets(); i++) {
		Sommet* s = g->get_sommet(i);
		add_sommet(s->x, s->y, s->c);
	}
	
	// Copie les arètes.
	for (int i = 0; i < g->get_nb_aretes(); i++) {
		Arete* a = g->get_arete(i);
		add_arete(a->parent, a->enfant, a->c, a->f);
	}

}

// Affecte le flot d'une arète dans le graphe.
void Graphe::set_flow(int s1, int s2, int f) {
	
	for (Arete* a : get_sommet(s1)->aretes) {
		if (a->enfant == s2) a->f = f;
	}

}

// Affecte le flot d'une arète dans le graphe.
void Graphe::set_flow(int a, int f) {
	if (a >= 0 && a < aretes.size()) aretes[a]->f = f;
}

// Fonction d'affichage du graphe.
void Graphe::draw(sf::RenderWindow& window, sf::Font& font, bool is_virtual, bool oriented) {
	int w = rect[2]-rect[0];
	int h = rect[3]-rect[1];

	// Affiche les sommets.
	for (Sommet* s : sommets) {

		// Détermine la couleur d'un sommet.
		sf::Color color = sf::Color::White;
		if (s->c < 0) color = sf::Color(44,155,188);
		else if (s->c > 0) {
			if (is_virtual) color = sf::Color::Red;
			else color = sf::Color(93, 202, 49);
		} else if (is_virtual) {
			for (Arete* a : s->aretes) {
				Sommet* s2 = (get_sommet(a->parent) == s) ? get_sommet(a->enfant) : get_sommet(a->parent);
				if (s2->c > 0) color = sf::Color(93, 202, 49);
			}
		}

		// Dessine le sommet.
		drawSommet(window, font, color, s->c,
			window.getSize().x/w*(s->x-rect[0]),
			window.getSize().y/h*(s->y-rect[1])
		);
	}

	// Affiche les arètes.
	for (Arete* a : aretes) {
		Arete* a2 = nullptr;
		for (Arete* aa : get_sommet(a->parent)->aretes) {
			if (aa->parent == a->enfant && aa->enfant == a->parent) a2 = aa;
		}

		// Coordonnées des deux extrémités de la ligne.
		int line[4] = {
			(int)(window.getSize().x/w)*(get_sommet(a->parent)->x-rect[0]),
			(int)(window.getSize().y/h)*(get_sommet(a->parent)->y-rect[1]),
			(int)(window.getSize().x/w)*(get_sommet(a->enfant)->x-rect[0]),
			(int)(window.getSize().y/h)*(get_sommet(a->enfant)->y-rect[1])
		};

		// Affiche une arète simple.
		if (a2 == nullptr) {
			drawArete(window, font, sf::Color::White,
				line[0], line[1], line[2], line[3],
	 			std::to_string(a->f) + "/" + std::to_string(a->c), oriented
			);

		// Affiche une arète double.
		} else if (a->parent >= a->enfant) {
			if (oriented) {
				drawDoubleArete(window, font, sf::Color::White,
					line[0], line[1], line[2], line[3],
					std::to_string(a->f) + "/" + std::to_string(a->c),
					std::to_string(a2->f) + "/" + std::to_string(a2->c)
				);

			} else {
				drawArete(window, font, sf::Color::White,
					line[0], line[1], line[2], line[3],
					std::to_string(a->f) + "/" + std::to_string(a->c) + ";" +
					std::to_string(a2->f) + "/" + std::to_string(a2->c)
				);
			}

		}
		
	}

}

// Dessine le graphe et en fait un screenshot.
// /!/ Ecrase l'affichage déjà présent dans la fenètre /!/
void Graphe::screenshot(sf::RenderWindow& window, sf::Font& font, std::string name, bool is_virtual, bool oriented) {

	// Dessine le graphe.
	window.clear();
	draw(window, font, is_virtual, oriented);
	window.display();

	// Enregistre un screenshot.
	sf::Texture texture;
	texture.create(window.getSize().x, window.getSize().y);
	texture.update(window);
	texture.copyToImage().saveToFile("screenshot/" + name + ".jpg");

}

// Calcule la zone sur laquelle afficher le graphe.
void Graphe::gen_rect() {
	if (get_nb_sommets() != 0) {

		Sommet* s = get_sommet(0);
		rect[0] = s->x-1;
		rect[1] = s->y-1;
		rect[2] = s->x+1;
		rect[3] = s->y+1;

		for (int i = 1; i < get_nb_sommets(); i++) {
			s = get_sommet(i);
			if (s->x <= rect[0]) rect[0] = s->x-1;
			if (s->y <= rect[1]) rect[1] = s->y-1;
			if (s->x >= rect[2]) rect[2] = s->x+1;
			if (s->y >= rect[3]) rect[3] = s->y+1;
		}

	}

}

void Graphe::drawPath(sf::RenderWindow& window, chemin* ch, float width) {
	// Dessine le chemin dans ce graphe désigné par chemin
	int w = rect[2]-rect[0];
	int h = rect[3]-rect[1];


	for (auto it = std::begin(*ch); it != std::end(*ch); ++it)
	{
		auto next = it;
		next++;
		if (next != std::end(*ch))
		{

			drawHeavyLine(window, sf::Color::White,
				(int)(window.getSize().x/w)*(get_sommet(*it)->x-rect[0]),
				(int)(window.getSize().y/h)*(get_sommet(*it)->y-rect[1]),
				(int)(window.getSize().x/w)*(get_sommet(*next)->x-rect[0]),
				(int)(window.getSize().y/h)*(get_sommet(*next)->y-rect[1]), width);
		}
	}
}

std::vector<Sommet*> Graphe::get_sommets(){
	//  Retourne la copie des sommets dans le graphe

	return sommets;
}

std::vector<int> Graphe::get_successeurs(int s) {
	// Renvoie la liste des successeur de s (pas testée)

	Sommet* s_e = get_sommet(s);

	std::vector<int> res(0);

	for (int i = 0; i < s_e->aretes.size(); ++i)
	{
		res.push_back(s_e->aretes[i]->enfant);
	}

	return res;
}
