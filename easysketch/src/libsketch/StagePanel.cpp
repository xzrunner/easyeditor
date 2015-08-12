#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteOP.h"

namespace libsketch
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::MultiSpritesImpl(GetStageImpl())
{
	StageCanvas* canvas = new StageCanvas(this, GetStageImpl(), this, library);
	SetCanvas(canvas);
	library->SetCanvas(canvas);

	SetEditOP(new ArrangeSpriteOP(this));

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), this, library));
}

StagePanel::~StagePanel()
{
}

void StagePanel::Clear()
{
	d2d::EditPanel::Clear();
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite, int idx)
{
	d2d::MultiSpritesImpl::InsertSprite(sprite);

	sprite->Retain();
	m_sprites.push_back(sprite);
	GetCanvas()->SetDirty();
	return true;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	d2d::MultiSpritesImpl::RemoveSprite(sprite);

	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (m_sprites[i] == sprite) {
			sprite->Release();
			m_sprites.erase(m_sprites.begin() + i);
			GetCanvas()->SetDirty();
			return true;
		}
	}
	return false;
}

bool StagePanel::ClearAllSprite()
{
	d2d::MultiSpritesImpl::ClearAllSprite();

	bool ret = !m_sprites.empty();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
	m_sprites.clear();
	if (ret) {
		GetCanvas()->SetDirty();
	}
	return ret;
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, d2d::DataTraverseType type, 
								 bool order) const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		bool fetch_next;
		visitor.Visit(m_sprites[i], fetch_next);
		if (!fetch_next) {
			break;
		}
	}
}

ivec2 StagePanel::TransPos3ProjectToScreen(const vec3& proj) const
{
	const StageCanvas* canvas = static_cast<const StageCanvas*>(GetCanvas());
	return canvas->TransPos3ProjectToScreen(proj);
}

vec3 StagePanel::TransPos3ScreenToProject(const ivec2& scr, float proj_z) const
{
	const StageCanvas* canvas = static_cast<const StageCanvas*>(GetCanvas());
	return canvas->TransPos3ScreenToProject(scr, proj_z);	
}

}