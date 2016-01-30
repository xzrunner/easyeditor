#include "BBFactory.h"
#include "AABB.h"
#include "OBB.h"

namespace ee
{

BoundingBox* BBFactory::CreateBB(BB_TYPE type)
{
	BoundingBox* bb = NULL;
	switch (type)
	{
	case e_aabb:
		bb = new AABB;
		break;
	case e_obb:
		bb = new OBB;
		break;
	}
	return bb;
}

}