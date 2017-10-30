#include "DeleteLayerAOP.h"
#include "Layer.h"
#include "LayerList.h"
#include "FetchAllVisitor.h"
#include "shape_msg.h"
#include "sprite_msg.h"

#include <algorithm>

namespace ee
{

DeleteLayerAOP::DeleteLayerAOP(LayerList* list, Layer* layer)
	: m_list(list)
{
	layer->TraverseSprite(FetchAllRefVisitor<Sprite>(m_sprs));

	layer->TraverseShape(FetchAllRefVisitor<Shape>(m_shapes));

	layer->AddReference();
	m_layer = layer;
}

DeleteLayerAOP::~DeleteLayerAOP()
{
	m_layer->RemoveReference();
}

void DeleteLayerAOP::Undo()
{
	m_list->Insert(m_layer);

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprs[i]);
	}
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		InsertShapeSJ::Instance()->Insert(m_shapes[i]);
	}
}

void DeleteLayerAOP::Redo()
{
	m_list->Remove(m_layer);

	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprs[i]);
	}
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		RemoveShapeSJ::Instance()->Remove(m_shapes[i]);
	}
}

Json::Value DeleteLayerAOP::Store(const std::vector<SprPtr>& sprs) const
{
	return Json::Value();
}

}