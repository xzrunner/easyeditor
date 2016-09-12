#include "NodeScale9.h"
#include "ImportStream.h"
#include "Allocator.h"

namespace simp
{

NodeScale9::NodeScale9(Allocator& alloc, ImportStream& is)
{
	type = is.UInt8();

	int n = 0;
	switch (type) {
	case 1:
		n = 9;
		break;
	case 2: case 3:
		n = 3;
		break;
	case 4:
		n = 6;
		break;
	case 5:
		n = 8;
		break;
	}

	alloc.Alloc(sizeof(Grid) * n);	

	for (int i = 0; i < n; ++i)
	{
		Grid& g = grids[i];
		g.sym = is.UInt32();
		g.dir = is.UInt16();
		g.mirror = is.UInt16();
	}
}

int NodeScale9::Size()
{
	return sizeof(NodeScale9) - sizeof(Grid);
}

int NodeScale9::GridSize()
{
	return sizeof(Grid);
}

}