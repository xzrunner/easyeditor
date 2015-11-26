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

	SetEditOP(new ArrangeSpriteOP(this));

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), library));

	d2d::InsertSpriteSJ::Instance()->Register(this);
	d2d::RemoveSpriteSJ::Instance()->Register(this);
	d2d::ClearSpriteSJ::Instance()->Register(this);
}

StagePanel::~StagePanel()
{
	d2d::InsertSpriteSJ::Instance()->UnRegister(this);
	d2d::RemoveSpriteSJ::Instance()->UnRegister(this);
	d2d::ClearSpriteSJ::Instance()->UnRegister(this);
}

void StagePanel::Notify(int sj_id, void* ud)
{
	d2d::MultiSpritesImpl::Notify(sj_id, ud);

	switch (sj_id)
	{
	case d2d::MSG_INSERT_SPRITE:
		Insert(((d2d::InsertSpriteSJ::Params*)ud)->spr);
		break;
	case d2d::MSG_REMOVE_SPRITE:
		Remove((d2d::ISprite*)ud);
		break;
	case d2d::MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
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

void StagePanel::Insert(d2d::ISprite* spr)
{
	spr->Retain();
	m_sprites.push_back(spr);
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Remove(d2d::ISprite* spr)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (m_sprites[i] == spr) {
			spr->Release();
			m_sprites.erase(m_sprites.begin() + i);
			d2d::SetCanvasDirtySJ::Instance()->SetDirty();
			return;
		}
	}
}

void StagePanel::Clear()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
	m_sprites.clear();
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

}