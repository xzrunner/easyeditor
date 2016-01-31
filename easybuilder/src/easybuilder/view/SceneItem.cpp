
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

	//m_bitmap = new ee::Bitmap(
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

void SceneItem::draw(const ee::Matrix& mt,
					 const ee::Colorf& mul, 
					 const ee::Colorf& add,
					 const ee::ISprite* sprite/* = NULL*/) const
{
}

ee::Rect SceneItem::getSize(const ee::ISprite* sprite /*= NULL*/) const
{
	return ee::Rect();
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
