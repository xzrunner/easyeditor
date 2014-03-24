
#include "SceneItem.h"
#include "Thumbnail.h"

#include "dataset/Game.h"
#include "dataset/Scene.h"
#include "dataset/Layer.h"

using namespace ebuilder;

SceneItem::SceneItem(Scene* scene)
	: m_scene(scene)
{
	m_name = m_scene->m_name;

	//m_bitmap = new d2d::Bitmap(
	//	new wxBitmap(
	//	Game::WIDTH * Thumbnail::SCALE, 
	//	Game::HEIGHT * Thumbnail::SCALE)
	//	);
}

SceneItem::~SceneItem()
{
	delete m_scene;
}

SceneItem* SceneItem::clone()
{
	return NULL;
}

void SceneItem::loadFromTextFile(std::ifstream& fin)
{
}

void SceneItem::storeToTextFile(std::ofstream& fout) const
{
}

void SceneItem::reloadTexture() const
{
}

void SceneItem::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
					 const d2d::ISprite* sprite/* = NULL*/) const
{
}

d2d::Rect SceneItem::getSize(const d2d::ISprite* sprite /*= NULL*/) const
{
	return d2d::Rect();
}

void SceneItem::refresh()
{
	d2d::ISymbol::refresh();
	refreshThumbnail();
}


const wxString& SceneItem::getName() const
{
	return m_name;
}

void SceneItem::setName(const wxString& name)
{
	m_name = name;
	m_scene->m_name = name;
}

void SceneItem::refreshThumbnail()
{
	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.SetBackground(wxBrush(*wxWHITE));
	memDC.Clear();

	const std::vector<Layer*>& layers = m_scene->getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		Thumbnail::draw(layers[i]->getActors(), memDC);

	memDC.SelectObject(wxNullBitmap);
}
