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
	layer->TraverseSprite(FetchAllVisitor<Sprite>(m_sprs));
	for_each(m_sprs.begin(), m_sprs.end(), cu::AddRefFunctor<Sprite>());

	layer->TraverseShape(FetchAllVisitor<Shape>(m_shapes));
	for_each(m_shapes.begin(), m_shapes.end(), cu::AddRefFunctor<Shape>());

	layer->AddReference();
	m_layer = layer;
}

DeleteLayerAOP::~DeleteLayerAOP()
{
	for_each(m_sprs.begin(), m_sprs.end(), cu::RemoveRefFunctor<Sprite>());
	for_each(m_shapes.begin(), m_shapes.end(), cu::RemoveRefFunctor<Shape>());
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

Json::Value DeleteLayerAOP::Store(const std::vector<Sprite*>& sprs) const
{
	return Json::Value();
}

}