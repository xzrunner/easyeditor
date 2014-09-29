
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

void SceneItem::reloadTexture() const
{
}

void SceneItem::draw(const d2d::Matrix& mt,
					 const d2d::Colorf& mul, 
					 const d2d::Colorf& add,
					 const d2d::ISprite* sprite/* = NULL*/) const
{
}

d2d::Rect SceneItem::getSize(const d2d::ISprite* sprite /*= NULL*/) const
{
	return d2d::Rect();
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
