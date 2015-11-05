#ifndef _DRAG2D_QUERY_SELECTED_SPRITE_LAYER_SJ_H_
#define _DRAG2D_QUERY_SELECTED_SPRITE_LAYER_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class QuerySelectedSpriteLayerSJ : public Subject
{
public:
	int Query();

	static QuerySelectedSpriteLayerSJ* Instance();

private:
	QuerySelectedSpriteLayerSJ(int id);

private:
	static QuerySelectedSpriteLayerSJ* m_instance;

}; // QuerySelectedSpriteLayerSJ

}

#endif // _DRAG2D_QUERY_SELECTED_SPRITE_LAYER_SJ_H_