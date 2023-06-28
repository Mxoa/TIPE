#ifndef GRAPH_H
#define GRAPH_H



#include <SFML/Graphics.hpp>
#include <ostream>
#include <vector>
#include <list>
#include <iostream>


// Désigne une suite de sommets dans un graphe Graphe, s'ils sont reliés par des arêtes, ils forment un chemin
typedef std::list<int> chemin;


// Structure d'arète pour un graphe orienté.
// Contient un sommet parent, un sommet enfant, une capacité et un flot.
struct Arete {
	int parent;
	int enfant;
	int c;
	int f;
	Arete(int p, int e, int c, int f = 0) {this->parent = p; this->enfant = e; this->c = c; this->f = f;}
	Arete() {this->parent = -1; this->enfant = -1; this->c = 0; this->f = 0;}
};
typedef struct Arete Arete;
bool operator==(const Arete& u, const Arete& v);

// Structure de sommet pour un graphe orienté.
// Contient des coordonnées, la consommation c du sommet et une listes d'arètes. 
struct Sommet {
	int x;
	int y;
	int c;
	std::vector<Arete*> aretes;
	Sommet(int x, int y, int c) {this->x = x; this->y = y; this->c = c;}
};
typedef struct Sommet Sommet;
bool operator==(const Sommet& u, const Sommet& v);
std::ostream& operator<<(std::ostream& out, Sommet const& s);



// Type pour les chemins.
typedef std::vector<int> Chemin; 



// Classe modélisant un graphe orienté pondéré par une capacité et un flot.
class Graphe {

public :

	// Constructeur, destructeur.
	Graphe(std::vector<Sommet*> sommets);
	~Graphe();

	// Fonction d'ajout/suppression d'arètes et de sommets.
	void add_arete(int s1, int s2, int c, int f = 0);
	void del_arete(int s1, int s2);
	void del_arete(int id);
	void add_sommet(int x, int y, int c);
	void add_sommet(Sommet* sommet);
	void del_sommet(int s);

	// Ajoute un deuxième graphe à celui-ci, on suppose qu'il n'y a pas de sommet en commun
	void add_graphe(Graphe* other);

	
	// Mise à zéro du flot.
	void zeroFlow();

	// Getters.
	Sommet* get_sommet(unsigned int id);
	int get_nb_sommets();
	int get_capacity(int s1, int s2);
	int get_flow(int s1, int s2);
	int get_flow(int a);
	Graphe* gen_residual_graph();
	Graphe* copy();
	int get_nb_aretes();
	Arete* get_arete(unsigned int id);
	int get_consommation();
	std::vector<Sommet*> get_sommets();
	std::vector<Arete*> get_aretes();
	std::vector<int> get_successeurs(int s);

	// Setters.
	void copy_from(Graphe* g);
	void set_flow(int s1, int s2, int f);
	void set_flow(int a, int f);

	// Fonctions d'affichage.
	void draw(sf::RenderWindow& window, sf::Font& font, bool is_virtual = false, bool oriented = true);
	void screenshot(sf::RenderWindow& window, sf::Font& font, std::string name, bool is_virtual = false, bool oriented = true);
	void drawPath(sf::RenderWindow& window, chemin* ch, float width);

private :
	
	// Listes de sommets et d'arètes.
	std::vector<Sommet*> sommets;
	std::vector<Arete*> aretes;

	// Fonction auxilière pour l'affichage.
	void gen_rect();
	int rect[4];

};



#endif