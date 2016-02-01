#ifndef _EASYSCALE9_COMPOSE_GRIDS_H_
#define _EASYSCALE9_COMPOSE_GRIDS_H_

#include <ee/Vector.h>

namespace escale9
{

class ComposeGrids
{
public:
	static void Draw();

	static void Query(const ee::Vector& pos, int* col, int* row);

	static ee::Vector GetGridCenter(int col, int row);

}; // ComposeGrids

}

#endif // _EASYSCALE9_COMPOSE_GRIDS_H_