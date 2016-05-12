#ifndef _EASYSCALE9_COMPOSE_GRIDS_H_
#define _EASYSCALE9_COMPOSE_GRIDS_H_

#include <SM_Vector.h>

namespace escale9
{

class ComposeGrids
{
public:
	static void Draw();

	static void Query(const sm::vec2& pos, int* col, int* row);

	static sm::vec2 GetGridCenter(int col, int row);

}; // ComposeGrids

}

#endif // _EASYSCALE9_COMPOSE_GRIDS_H_