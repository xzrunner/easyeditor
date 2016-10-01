#include "StagePanel.h"
#include "StageCanvas.h"
#include "KeysPanel.h"

#include "edit/ArrangeSpriteOP.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/SpriteUserData.h"
#include "dataset/DataMgr.h"
#include "view/ViewMgr.h"
#include "view/LibraryPanel.h"
#include "message/messages.h"

#include <ee/panel_msg.h>
#include <ee/sprite_msg.h>
#include <ee/subject_id.h>

#include <easyparticle3d.h>

#include <sprite2/RenderParams.h>

namespace eanim
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, MultiSpritesImpl(GetStageImpl())
	, m_layer_idx(-1)
	, m_frame_idx(-1)
	, m_frame(NULL)
	, m_refresh(false)
{
//	m_editop = new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(this, this);
	SetEditOP(new ArrangeSpriteOP(this));
	SetCanvas(new StageCanvas(this)); 

	SetDropTarget(new StageDropTarget(this, ViewMgr::Instance()->library));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnMenuAddJointNode, this, Menu_AddJointNode);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnMenuDelJointNode, this, Menu_DelJointNode);

	RegistSubject(ee::RefreshPanelSJ::Instance());
	RegistSubject(ee::ReorderSpriteSJ::Instance());
	RegistSubject(ee::ReorderSpriteMostSJ::Instance());
	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
	RegistSubject(ee::ClearSpriteSJ::Instance());

	RegistSubject(SetSelectedSJ::Instance());
	RegistSubject(RemoveLayerSJ::Instance());
}

bool StagePanel::UpdateStage()
{
	bool dirty = false;

	if (m_refresh) {
		m_refresh = false;
		return true;
	}

	CheckUpdateVisitor visitor;
	TraverseSprites(visitor, ee::DT_ALL, true);
	if (visitor.NeedUpdate()) {
		dirty = true;
	}
	return dirty;
}

void StagePanel::TraverseSprites(ee::Visitor<ee::Sprite>& visitor, 
								 ee::DataTraverseType type/* = ee::e_allExisting*/,
								 bool order/* = true*/) const
{
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(i);
		if ((type == ee::DT_EDITABLE && !layer->IsEditable()) ||
			(type == ee::DT_VISIBLE && !layer->IsVisible()))
			continue;

		KeyFrame* frame = layer->GetCurrKeyFrame(m_frame_idx + 1);
		if (!frame) continue;

		if (order)
		{
			for (int i = 0, n = frame->Size(); i < n; ++i) {
				bool next;
				visitor.Visit(const_cast<ee::Sprite*>(frame->GetSprite(i)), next);
				if (!next) break;
			}
		}
		else
		{
			for (int i = frame->Size() - 1; i >= 0; --i) {
				bool next;
				visitor.Visit(const_cast<ee::Sprite*>(frame->GetSprite(i)), next);
				if (!next) break;
			}
		}
	}
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_REFRESH_PANEL:
		m_refresh = true;
		break;

	case ee::MSG_REORDER_SPRITE:
		{
			ee::ReorderSpriteSJ::Params* p = (ee::ReorderSpriteSJ::Params*)ud;
			Reorder(p->spr, p->up);
		}
		break;
	case ee::MSG_REORDER_SPRITE_MOST:
		{
			ee::ReorderSpriteMostSJ::Params* p = (ee::ReorderSpriteMostSJ::Params*)ud;
			ReorderMost(p->spr, p->up);
		}
		break;
	case ee::MSG_SORT_SPRITES:
		{
			std::vector<ee::Sprite*>& sprs = *(std::vector<ee::Sprite*>*)ud;
			if (m_frame) {
				m_frame->Sort(sprs);
			}
		}
		break;
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			Insert(p->spr, p->idx);
		}
		break;
	case ee::MSG_REMOVE_SPRITE:
		Remove((ee::Sprite*)ud);
		break;
	case ee::MSG_CLEAR_SPRITE:
		{
			DataMgr::Instance()->GetLayers().Clear();
			SetSelectedSJ::Instance()->Set(-1, -1);

			ee::SetCanvasDirtySJ::Instance()->SetDirty();

			// 	Context* context = Context::Instance();
			// 	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
			// 	frame->clear();
		}
		break;

	case MSG_REMOVE_LAYER:
		ClearSelectedSprite();
		m_refresh = true;
		break;

	case MSG_SET_CURR_FRAME:
		{
			SetSelectedSJ::Position* cf = (SetSelectedSJ::Position*)ud;
			if (cf->layer == -1 && cf->frame == -1) {
				m_layer_idx = m_frame_idx = -1;
				m_frame = NULL;
			} else {
				m_refresh = true;
				GetEditOP()->Clear();
				if (cf->layer != -1) {
					m_layer_idx = cf->layer;
				}
				if (cf->frame != -1) {
					m_frame_idx = cf->frame;
				}
				if (m_layer_idx != -1) {
					Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(m_layer_idx);
					m_frame = layer->GetCurrKeyFrame(m_frame_idx + 1);
				} else {
					m_frame = NULL;
				}
			}

			s2::Particle3d::Instance()->BufferClear();
			if (m_frame) {
				m_frame->OnActive();
			}
		}
		break;
	}
}

void StagePanel::OnMenuAddJointNode(wxCommandEvent& event)
{
	GetEditOP()->OnPopMenuSelected(Menu_AddJointNode);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::OnMenuDelJointNode(wxCommandEvent& event)
{
	GetEditOP()->OnPopMenuSelected(Menu_DelJointNode);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Reorder(ee::Sprite* spr, bool up)
{
	if (m_frame && m_frame->Reorder(spr, up)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void StagePanel::ReorderMost(ee::Sprite* spr, bool up)
{
	if (m_frame && m_frame->ReorderMost(spr, up)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void StagePanel::Insert(ee::Sprite* spr, int idx)
{
	if (spr->GetUserData()) {
		InsertWithUD(spr, idx);
	} else {
		InsertWithoutUD(spr, idx);
	}
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Remove(ee::Sprite* spr)
{
	if (m_frame && m_frame->Remove(spr)) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void StagePanel::InsertWithUD(ee::Sprite* spr, int idx)
{
	SpriteUserData* ud = (SpriteUserData*)spr->GetUserData();
	assert(ud);

	if (ud->layer_idx != -1) {
		assert(ud->layer_idx != -1);
		while (ud && ud->layer_idx >= DataMgr::Instance()->GetLayers().Size()) {
			InsertLayerSJ::Instance()->Insert();
			SetSelectedSJ::Instance()->Set(0, 0);
		}

		assert(m_frame);
		m_frame->Insert(spr, idx);

		ud->layer = DataMgr::Instance()->GetLayers().GetLayer(ud->layer_idx);
		ud->frame = m_frame;
	} else {
		assert(ud->frame);
		ud->frame->Insert(spr, idx);
	}
}

void StagePanel::InsertWithoutUD(ee::Sprite* spr, int idx)
{
	if (m_frame) {
		m_frame->Insert(spr, idx);
	}
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::CheckUpdateVisitor
//////////////////////////////////////////////////////////////////////////

StagePanel::CheckUpdateVisitor::
CheckUpdateVisitor() 
	: m_update(false) 
{}

void StagePanel::CheckUpdateVisitor::
Visit(ee::Sprite* spr, bool& next)
{
	if (spr->Update(s2::RenderParams())) {
		m_update = true;
	}
	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::StageDropTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::StageDropTarget::
StageDropTarget(StagePanel* stage, ee::LibraryPanel* library)
	: ee::StageDropTarget(stage, stage->GetStageImpl(), library)
	, m_stage(stage)
{
}

void StagePanel::StageDropTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	if (m_stage->IsCurrFrameValid()) {
		ee::StageDropTarget::OnDropText(x, y, text);
	}
}

void StagePanel::StageDropTarget::
OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	if (m_stage->IsCurrFrameValid()) {
		ee::StageDropTarget::OnDropFiles(x, y, filenames);
	}	
}

} // eanim