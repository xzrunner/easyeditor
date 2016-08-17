#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteOP.h"

#include <ee/StageDropTarget.h>
#include <ee/sprite_msg.h>
#include <ee/Visitor.h>
#include <ee/Sprite.h>
#include <ee/subject_id.h>
#include <ee/panel_msg.h>

namespace esketch
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::MultiSpritesImpl(GetStageImpl())
{
	StageCanvas* canvas = new StageCanvas(this, GetStageImpl(), this, library);
	SetCanvas(canvas);

	SetEditOP(new ArrangeSpriteOP(this));

	SetDropTarget(new ee::StageDropTarget(this, GetStageImpl(), library));

	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
	RegistSubject(ee::ClearSpriteSJ::Instance());
}

void StagePanel::TraverseSprites(ee::Visitor<ee::Sprite>& visitor, ee::DataTraverseType type, 
								 bool order) const
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		bool fetch_next;
		visitor.Visit(m_sprs[i], fetch_next);
		if (!fetch_next) {
			break;
		}
	}
}

sm::ivec2 StagePanel::TransPos3ProjectToScreen(const sm::vec3& proj) const
{
	const StageCanvas* canvas = static_cast<const StageCanvas*>(GetCanvas());
	return canvas->TransPos3ProjectToScreen(proj);
}

sm::vec3 StagePanel::TransPos3ScreenToProject(const sm::ivec2& scr, float proj_z) const
{
	const StageCanvas* canvas = static_cast<const StageCanvas*>(GetCanvas());
	return canvas->TransPos3ScreenToProject(scr, proj_z);	
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		Insert(((ee::InsertSpriteSJ::Params*)ud)->spr);
		break;
	case ee::MSG_REMOVE_SPRITE:
		Remove((ee::Sprite*)ud);
		break;
	case ee::MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
}

void StagePanel::Insert(ee::Sprite* spr)
{
	spr->AddReference();
	m_sprs.push_back(spr);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Remove(ee::Sprite* spr)
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		if (m_sprs[i] == spr) {
			spr->RemoveReference();
			m_sprs.erase(m_sprs.begin() + i);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			return;
		}
	}
}

void StagePanel::Clear()
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->RemoveReference();
	}
	m_sprs.clear();
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}