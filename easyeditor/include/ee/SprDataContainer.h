#ifndef _EASYEDITOR_SPR_DATA_CONTAINER_H_
#define _EASYEDITOR_SPR_DATA_CONTAINER_H_

#include "DataContainer.h"

namespace ee
{

class Sprite;

class SprDataContainer : public DataContainer
{
public:
	virtual bool Sort(std::vector<Sprite*>& sprites) = 0;

}; // SprDataContainer

}

#endif // _EASYEDITOR_SPR_DATA_CONTAINER_H_