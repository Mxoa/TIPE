#include "union_find.h"



// Constructeur.
UnionFind::UnionFind(int n) {

	for (int i = 0; i < n; i++) p.push_back(i);
	for (int i = 0; i < n; i++) r.push_back(0);

}

// Destructeur.
UnionFind::~UnionFind() {
}

// Renvoie le représentant de la classe de x.
int UnionFind::find(int x) {

	if (p[x] != x) {
		p[x] = find(p[x]);
	}
	return p[x];

}

// Joins les classes de x et de y.
void UnionFind::join(int x, int y) {

	int px = find(x);
	int py = find(y);
	if (px != py) {

		if (r[px] < r[py]) {
			p[px] = py;

		} else {
			p[py] = px;
			if (r[px] == r[py]) r[px]++;
		}

	}

}
