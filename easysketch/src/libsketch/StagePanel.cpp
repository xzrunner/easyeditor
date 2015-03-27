#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteOP.h"

namespace libsketch
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(parent)
{
	m_canvas = new StageCanvas(this, this, library);
	library->setCanvas(m_canvas);

	m_editOP = new ArrangeSpriteOP(this);

	SetDropTarget(new d2d::StageDropTarget(this, this, library));
}

StagePanel::~StagePanel()
{
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();
}

void StagePanel::traverseSprites(d2d::IVisitor& visitor, d2d::TraverseType type, 
								 bool order) const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		bool fetch_next;
		visitor.visit(m_sprites[i], fetch_next);
		if (!fetch_next) {
			break;
		}
	}
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (m_sprites[i] == sprite) {
			sprite->Release();
			m_sprites.erase(m_sprites.begin() + i);
		}
	}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	sprite->Retain();
	m_sprites.push_back(sprite);

	m_canvas->Refresh();
}

void StagePanel::clearSprites()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
	m_sprites.clear();
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{

}

ivec2 StagePanel::TransPos3ProjectToScreen(const vec3& proj) const
{
	StageCanvas* canvas = static_cast<StageCanvas*>(m_canvas);
	return canvas->TransPos3ProjectToScreen(proj);
}

vec3 StagePanel::TransPos3ScreenToProject(const ivec2& scr, float proj_z) const
{
	StageCanvas* canvas = static_cast<StageCanvas*>(m_canvas);
	return canvas->TransPos3ScreenToProject(scr, proj_z);	
}

}