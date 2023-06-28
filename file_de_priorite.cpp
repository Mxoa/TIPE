#include "file_de_priorite.hpp"

bool comp(doublet const& a, doublet const& b) {
	return a.second < b.second;
}

std::ostream& operator<<(std::ostream& out, doublet const& a) {
	out << "(" << a.first << ", " << a.second << ")";
	return out;
}



FileDePriorite::FileDePriorite() : m_sommets(), m_nb_elem(0) {

}

bool FileDePriorite::empty() const {
	return m_nb_elem == 0;
}

void FileDePriorite::enfile_prio(int elem, float prio) {
	m_sommets.push_front(std::make_pair(elem, prio));
	m_sommets.sort(comp);
	//affiche();
	m_nb_elem++;
}

int FileDePriorite::get_min() {
	// Hyp : !(this->empty())

	return m_sommets.front().first;
}

int FileDePriorite::extract_min() {

	int u = this->get_min();
	this->remove_min();
	return u;
}

void FileDePriorite::remove_min() {
	// Hyp : !(this->empty())
	m_sommets.pop_front();
	m_nb_elem--;
}

void FileDePriorite::affiche() {
	// Affiche la file de priorité
	print_list(&m_sommets);
}


void FileDePriorite::reprio(int elem, float nprio) {

	std::list<doublet> tmp;

	for (doublet elt : m_sommets)
	{
		if (elt.first == elem)
		{
			elt.second = nprio;
		}

		tmp.push_front(elt);
	}

	m_sommets = tmp;

	m_sommets.sort(comp);
}

bool FileDePriorite::is_in(int elem) {
	bool res = false;

	for (doublet elt : m_sommets)
	{
		if (elt.first == elem)
		{
			res = true;
		}
	}

	return res;
}