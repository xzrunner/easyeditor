
#include "LayerItem.h"
#include "Thumbnail.h"

#include "dataset/Game.h"
#include "dataset/Layer.h"

using namespace ebuilder;

LayerItem::LayerItem(Layer* layer)
	: m_layer(layer)
{
	m_name = m_layer->name;

	//m_bitmap = new d2d::Bitmap(
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

void LayerItem::loadFromTextFile(std::ifstream& fin)
{
}

void LayerItem::storeToTextFile(std::ofstream& fout) const
{
}

void LayerItem::reloadTexture() const
{
}

void LayerItem::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
					 const d2d::ISprite* sprite/* = NULL*/) const
{
}

d2d::Rect LayerItem::getSize(const d2d::ISprite* sprite /*= NULL*/) const
{
	return d2d::Rect();
}

void LayerItem::refresh()
{
	d2d::ISymbol::refresh();
	refreshThumbnail();
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

void LayerItem::refreshThumbnail()
{
	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.SetBackground(wxBrush(*wxWHITE));
	memDC.Clear();

	Thumbnail::draw(m_layer->getActors(), memDC);

	memDC.SelectObject(wxNullBitmap);
}