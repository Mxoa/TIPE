#ifndef UNION_FIND_H
#define UNION_FIND_H



#include <vector>



// Class UnionFind : Permet d'utiliser la structure Union-Find pour des éléments dans [0..n-1].
class UnionFind {

public :
	
	// Constructeur, destructeur.
	UnionFind(int n);
	~UnionFind();

	// Fonctions de recherche et d'union.
	int find(int x);
	void join(int x, int y);

private :
	
	// Liste des parents et des rangs.
	std::vector<int> p;
	std::vector<int> r;

};



#endif
