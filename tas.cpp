#include "tas.hpp"



Tas::Tas(int n) : m_tas(n), m_nb_elem(0) {

}

void Tas::add_elem(int elem, float prio) {
	// Ajoute l'élément elem au tas avec la priorité prio
	if (est_plein())
	{
		m_tas.resize(2*m_nb_elem, 0);
	}
	m_tas[m_nb_elem] = elem;
	m_nb_elem++;

	int u = m_nb_elem-1;

	while(u != pere(u) && m_tas[u] < m_tas[pere(u)]) {
		echanger(u, pere(u));
		u = pere(u);
	}
}

int Tas::get_min() const {
	return m_tas[0];
}

void Tas::remove_min() {
	// Supprime le minimum du tas (la case d'indice 0)

	m_nb_elem--;
	m_tas[0] = m_tas[m_nb_elem];

	// On a remit le dernier element en haut de l'arbre
	// Il faut le faire redescendre

	int imposteur = 0;
	// L' 'imposteur' est le noeud qui n'est pas à sa place
	bool ordone = false;

	while (!ordone) {
		int f_d = fils_droit(imposteur);
		int f_g = fils_gauche(imposteur);
		int p_d = m_tas[f_d];
		int p_g = m_tas[f_g];
		if (f_g >= m_nb_elem)
		{
			// Il n'y a pas de fils
			ordone = true;
		} else if(f_d >= m_nb_elem) {
			// Il n'y a qu'un seul fils (gauche) dans ce cas on compare et on échange au besoin
			if (p_g < m_tas[imposteur])
			{
				echanger(f_g, imposteur);
				imposteur = f_g;
			}
		} else {
			// Les deux fils existent
			if (p_g > p_d)
			{
				if (m_tas[imposteur] > p_g)
				{
					echanger(m_tas[imposteur], p_g);
					imposteur = f_g;
				}
			} else if (p_d > p_g) {
				if (m_tas[imposteur] > p_d)
				{
					echanger(m_tas[imposteur], p_d);
					imposteur = f_d;
				}
			}
		}
	}
}

int Tas::extract_min() {
	int min = get_min();
	remove_min();
	return min;
}

int Tas::fils_droit(int sommet) {
	return 2*sommet + 2;
}

int Tas::fils_gauche(int sommet) {
	return 2*sommet + 1;
}

int Tas::pere(int i) {
	if(i == 0) {
		return 0;
	} if (i % 2 == 0) {
		return i/2 - 1;
	} else {
		return (i-1)/2;
	}
}

void Tas::echanger(int i, int j) {
	assert(i < m_nb_elem && j < m_nb_elem);
	int tmp = m_tas[i];
	m_tas[i] = m_tas[j];
	m_tas[j] = tmp;
}


bool Tas::est_plein() const {
	return m_nb_elem == (int) m_tas.size();
}

void Tas::show() {
	// Affiche le tas
	int deux_puissance = 1; // compteur de puissance de 2^i
	int k = 0;
	int inter_puissance = 0;

	while(k < m_nb_elem) {
		std::cout << m_tas[k];
		inter_puissance++;
		

		if (inter_puissance == deux_puissance)
		{
			std::cout << '\n';
			for (int j = 0; j < deux_puissance; ++j)
			{
				std::cout << "|\\";
			}
			std::cout << '\n';

			deux_puissance *= 2;
			inter_puissance = 0;


		}

		
		k++;
	}

	std::cout << std::endl;
}