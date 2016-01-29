#ifndef _DRAG2D_DELETE_LAYER_AOP_H_
#define _DRAG2D_DELETE_LAYER_AOP_H_

#include "AbstractAtomicOP.h"

namespace d2d
{

class Layer;
class LayerList;
class IShape;

class DeleteLayerAOP : public AbstractAtomicOP
{
public:
	DeleteLayerAOP(LayerList* list, Layer* layer);
	virtual ~DeleteLayerAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ISprite*>& sprites) const;

private:
	LayerList* m_list;

	Layer* m_layer;
	std::vector<ISprite*> m_sprites;
	std::vector<IShape*> m_shapes;

}; // DeleteLayerAOP

}

#endif // _DRAG2D_DELETE_LAYER_AOP_H_