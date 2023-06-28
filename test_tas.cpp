#include "tas.hpp"


int main()
{
	Tas t(6);

	t.add_elem(0, 0);
	t.add_elem(1, 1);
	t.add_elem(2, 2);
	t.add_elem(3, 3);
	t.add_elem(4, 4);
	t.add_elem(5, 5);
	t.add_elem(6, 6);

	t.show();

	return 0;
}
