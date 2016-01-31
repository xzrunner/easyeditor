
#include "LayerItem.h"
#include "Thumbnail.h"

#include "dataset/Game.h"
#include "dataset/Layer.h"

using namespace ebuilder;

LayerItem::LayerItem(Layer* layer)
	: m_layer(layer)
{
	m_name = m_layer->name;

	//m_bitmap = new ee::Bitmap(
	//	new wxBitmap(
	//	Game::WIDTH * Thumbnail::SCALE, 
	//	Game::HEIGHT * Thumbnail::SCALE)
	//	);
}

LayerItem::~LayerItem()
{
	delete m_layer;
}

LayerItem* LayerItem::clone()
{
	return NULL;
}

void LayerItem::reloadTexture() const
{
}

void LayerItem::draw(const ee::Matrix& mt,
					 const ee::Colorf& mul, 
					 const ee::Colorf& add,
					 const ee::ISprite* sprite/* = NULL*/) const
{
}

ee::Rect LayerItem::getSize(const ee::ISprite* sprite /*= NULL*/) const
{
	return ee::Rect();
}

const wxString& LayerItem::getName() const
{
	return m_name;
}

void LayerItem::setName(const wxString& name)
{
	m_name = name;
	m_layer->name = name;
}
