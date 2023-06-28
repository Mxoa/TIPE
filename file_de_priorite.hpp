#ifndef FILE_DE_PRIORITE
#define FILE_DE_PRIORITE

#include "outils.hpp"

/**
 * Implémente une file de priorité pour des entiers */


bool comp(doublet const& a, doublet const& b);
std::ostream& operator<<(std::ostream& out, doublet const& a);


class FileDePriorite {
public:
	FileDePriorite();
	bool empty() const;
	void enfile_prio(int elem, float prio);
	int get_min();
	int extract_min();
	void remove_min();
	void reprio(int elem, float prio);
	bool is_in(int elem);
	void affiche();

private:
	std::list<doublet> m_sommets;
	unsigned int m_nb_elem;
};

#endif