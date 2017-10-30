#ifndef _EASYEDITOR_DELETE_LAYER_AOP_H_
#define _EASYEDITOR_DELETE_LAYER_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"
#include "Shape.h"

namespace ee
{

class Layer;
class LayerList;

class DeleteLayerAOP : public AtomicOP
{
public:
	DeleteLayerAOP(LayerList* list, Layer* layer);
	virtual ~DeleteLayerAOP();

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<SprPtr>& sprs) const;

private:
	LayerList* m_list;

	Layer* m_layer;
	std::vector<SprPtr> m_sprs;
	std::vector<ShapePtr> m_shapes;

}; // DeleteLayerAOP

}

#endif // _EASYEDITOR_DELETE_LAYER_AOP_H_