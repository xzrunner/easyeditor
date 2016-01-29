#include "DeleteLayerAOP.h"

#include "dataset/Layer.h"
#include "view/LayerList.h"
#include "common/visitors.h"
#include "message/InsertSpriteSJ.h"
#include "message/InsertShapeSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/RemoveShapeSJ.h"

#include <algorithm>

namespace d2d
{

DeleteLayerAOP::DeleteLayerAOP(LayerList* list, Layer* layer)
	: m_list(list)
{
	layer->TraverseSprite(FetchAllVisitor<ISprite>(m_sprites));
	for_each(m_sprites.begin(), m_sprites.end(), RetainObjectFunctor<ISprite>());

	layer->TraverseSprite(FetchAllVisitor<IShape>(m_shapes));
	for_each(m_shapes.begin(), m_shapes.end(), RetainObjectFunctor<IShape>());

	layer->Retain();
	m_layer = layer;
}

DeleteLayerAOP::~DeleteLayerAOP()
{
	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<ISprite>());
	for_each(m_shapes.begin(), m_shapes.end(), ReleaseObjectFunctor<IShape>());
	m_layer->Release();
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

Json::Value DeleteLayerAOP::Store(const std::vector<ISprite*>& sprites) const
{
	return Json::Value();
}

}