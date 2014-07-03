
#include "Canvas.h"
#include "Panel.h"
#include "Scene.h"
#include "Item.h"

#include "view/LibraryPanel.h"

using namespace ebuilder::preview;

Canvas::Canvas(Panel* panel, ebuilder::LibraryPanel* libraryPanel)
	: d2d::OrthoCanvas(panel)
	, m_libraryPanel(libraryPanel)
{
}

Canvas::~Canvas()
{
}

void Canvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	m_libraryPanel->getMediaList()->reloadTexture();
}

void Canvas::onDraw()
{
	Scene* scene = static_cast<Panel*>(m_editPanel)->getScene();
	for (size_t i = 0, n = scene->m_items.size(); i < n; ++i)
		d2d::SpriteDraw::drawSprite(m_screen, scene->m_items[i]);
}
