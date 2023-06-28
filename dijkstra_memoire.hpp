#ifndef DIJK_MEM
#define DIJK_MEM

#include "graphe_m.hpp"
#include "file_de_priorite.hpp"

#define INFINITY 1000000

struct floatf
{
	float val;
	bool inf;
};

std::ostream& operator<<(std::ostream& flux, floatf const& z);

bool operator<(floatf const& x, floatf const& y);
bool operator>(floatf const& x, floatf const& y);
bool operator==(floatf const& x, floatf const& y);
bool operator<=(floatf const& x, floatf const& y);
bool operator>=(floatf const& x, floatf const& y);
floatf operator+(floatf const& x, float const& y);
floatf to_floatf(float x);



class Dijkstra_memoire
{
public:
	// Permet de trouver la distance minimale qui relie la ville au lotissement 
	Dijkstra_memoire(Graphe_m* g, ensemble_points* ville, ensemble_points* lotissement);
	chemin trouve_chemin(bool verbose = false);
	void affiche_distance() const;
	
	~Dijkstra_memoire();

private:

	void auto_resize();
	
	std::vector<floatf> m_distance_min; // Distance minimale à la ville
	std::vector<int> m_enfant_prefere; // Tableau des sommets à suivre pour arriver au lotissemnt
	std::vector<int> m_peres;
	Graphe_m* m_graphe;
	ensemble_points* m_ville;
	ensemble_points* m_loti;

};
#endif 