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
	layer->TraverseSprite(FetchAllVisitor<Sprite>(m_sprites));
	for_each(m_sprites.begin(), m_sprites.end(), cu::AddRefFonctor<Sprite>());

	layer->TraverseSprite(FetchAllVisitor<Shape>(m_shapes));
	for_each(m_shapes.begin(), m_shapes.end(), cu::AddRefFonctor<Shape>());

	layer->AddReference();
	m_layer = layer;
}

DeleteLayerAOP::~DeleteLayerAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), cu::RemoveRefFonctor<Sprite>());
	for_each(m_shapes.begin(), m_shapes.end(), cu::RemoveRefFonctor<Shape>());
	m_layer->RemoveReference();
}

void DeleteLayerAOP::Undo()
{
	m_list->Insert(m_layer);

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		InsertSpriteSJ::Instance()->Insert(m_sprites[i]);
	}
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		InsertShapeSJ::Instance()->Insert(m_shapes[i]);
	}
}

void DeleteLayerAOP::Redo()
{
	m_list->Remove(m_layer);

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(m_sprites[i]);
	}
	for (int i = 0, n = m_shapes.size(); i < n; ++i) {
		RemoveShapeSJ::Instance()->Remove(m_shapes[i]);
	}
}

Json::Value DeleteLayerAOP::Store(const std::vector<Sprite*>& sprites) const
{
	return Json::Value();
}

}