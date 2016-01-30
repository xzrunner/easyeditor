#ifndef _EASYEDITOR_BB_FACTORY_H_
#define _EASYEDITOR_BB_FACTORY_H_

#include "BBTypes.h"

namespace ee
{

class BoundingBox;

class BBFactory
{
public:
	static BoundingBox* CreateBB(BB_TYPE type);

}; // BBFactory

}

#endif // _EASYEDITOR_BB_FACTORY_H_