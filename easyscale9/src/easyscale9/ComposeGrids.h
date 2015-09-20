#ifndef _EASYSCALE9_COMPOSE_GRIDS_H_
#define _EASYSCALE9_COMPOSE_GRIDS_H_

#include <drag2d.h>

namespace escale9
{

class ComposeGrids
{
public:
	static void Draw();

	static void Query(const d2d::Vector& pos, int* col, int* row);

	static d2d::Vector GetGridCenter(int col, int row);

}; // ComposeGrids

}

#endif // _EASYSCALE9_COMPOSE_GRIDS_H_