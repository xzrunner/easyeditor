#include "StagePanel.h"
#include "StageCanvas.h"
#include "KeysPanel.h"

#include "edit/ArrangeSpriteOP.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/SpriteUserData.h"
#include "dataset/DataMgr.h"
#include "dataset/data_utility.h"
#include "view/ViewMgr.h"
#include "view/view_utility.h"
#include "message/messages.h"

namespace eanim
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, MultiSpritesImpl(GetStageImpl())
{
//	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this);
	SetEditOP(new ArrangeSpriteOP(this));
	SetCanvas(new StageCanvas(this)); 

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), ViewMgr::Instance()->library));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnMenuAddJointNode, this, Menu_AddJointNode);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::OnMenuDelJointNode, this, Menu_DelJointNode);

	RegistSubject(d2d::ReorderSpriteSJ::Instance());
	RegistSubject(d2d::ReorderSpriteMostSJ::Instance());
	RegistSubject(d2d::InsertSpriteSJ::Instance());
	RegistSubject(d2d::RemoveSpriteSJ::Instance());
	RegistSubject(d2d::ClearSpriteSJ::Instance());
	RegistSubject(RemoveLayerSJ::Instance());
}

bool StagePanel::Update(int version)
{
	CheckUpdateVisitor visitor(version);
	TraverseSprites(visitor, d2d::DT_ALL, true);
	return visitor.NeedUpdate();
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, 
								 d2d::DataTraverseType type/* = d2d::e_allExisting*/,
								 bool order/* = true*/) const
{
	int frame_idx = get_curr_frame_index();
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(i);
		if ((type == d2d::DT_EDITABLE && !layer->IsEditable()) ||
			(type == d2d::DT_VISIBLE && !layer->IsVisible()))
			continue;

		KeyFrame* frame = layer->GetCurrKeyFrame(frame_idx);
		if (!frame) continue;

		if (order)
		{
			for (int i = 0, n = frame->Size(); i < n; ++i) {
				bool hasNext;
				visitor.Visit(const_cast<d2d::ISprite*>(frame->GetSprite(i)), hasNext);
				if (!hasNext) break;
			}
		}
		else
		{
			for (int i = frame->Size() - 1; i >= 0; --i) {
				bool hasNext;
				visitor.Visit(const_cast<d2d::ISprite*>(frame->GetSprite(i)), hasNext);
				if (!hasNext) break;
			}
		}
	}
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case d2d::MSG_REORDER_SPRITE:
		{
			d2d::ReorderSpriteSJ::Params* p = (d2d::ReorderSpriteSJ::Params*)ud;
			Reorder(p->spr, p->up);
		}
		break;
	case d2d::MSG_REORDER_SPRITE_MOST:
		{
			d2d::ReorderSpriteMostSJ::Params* p = (d2d::ReorderSpriteMostSJ::Params*)ud;
			ReorderMost(p->spr, p->up);
		}
		break;
	case d2d::MSG_INSERT_SPRITE:
		{
			d2d::InsertSpriteSJ::Params* p = (d2d::InsertSpriteSJ::Params*)ud;
			Insert(p->spr);
		}
		break;
	case d2d::MSG_REMOVE_SPRITE:
		Remove((d2d::ISprite*)ud);
		break;
	case d2d::MSG_CLEAR_SPRITE:
		{
			DataMgr::Instance()->GetLayers().Clear();
			SetCurrFrameSJ::Instance()->Set(0, 0);

			d2d::SetCanvasDirtySJ::Instance()->SetDirty();

			// 	Context* context = Context::Instance();
			// 	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
			// 	frame->clear();
		}
		break;

	case MSG_REMOVE_LAYER:
		ClearSelectedSprite();
		break;
	}
}

void StagePanel::OnMenuAddJointNode(wxCommandEvent& event)
{
	GetEditOP()->OnPopMenuSelected(Menu_AddJointNode);
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::OnMenuDelJointNode(wxCommandEvent& event)
{
	GetEditOP()->OnPopMenuSelected(Menu_DelJointNode);
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Reorder(d2d::ISprite* spr, bool up)
{
	KeyFrame* frame = get_curr_frame();
	if (frame && frame->Reorder(spr, up)) {
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void StagePanel::ReorderMost(d2d::ISprite* spr, bool up)
{
	KeyFrame* frame = get_curr_frame();
	if (frame && frame->ReorderMost(spr, up)) {
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void StagePanel::Insert(d2d::ISprite* spr)
{
	if (spr->GetUserData()) {
		InsertWithUD(spr);
	} else {
		InsertWithoutUD(spr);
	}
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::Remove(d2d::ISprite* spr)
{
	KeyFrame* frame = get_curr_frame();
	if (frame && frame->Remove(spr)) {
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void StagePanel::InsertWithUD(d2d::ISprite* spr)
{
	SpriteUserData* ud = (SpriteUserData*)spr->GetUserData();
	assert(ud);

	assert(ud->layer_idx != -1);
	while (ud && ud->layer_idx >= DataMgr::Instance()->GetLayers().Size()) {
		InsertLayerSJ::Instance()->Insert();
	}

// 	int old_layer, old_frame;
// 	GetCurrFrameSJ::Instance()->Get(old_layer, old_frame);

	KeyFrame* frame = get_curr_frame();
	assert(frame);
	frame->Insert(spr);

	ud->layer = DataMgr::Instance()->GetLayers().GetLayer(ud->layer_idx);
	ud->frame = frame;

//	SetCurrFrameSJ::Instance()->Set(old_layer, old_frame);
}

void StagePanel::InsertWithoutUD(d2d::ISprite* spr)
{
	KeyFrame* frame = get_curr_frame();
	assert(frame);
	frame->Insert(spr);
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::CheckUpdateVisitor
//////////////////////////////////////////////////////////////////////////

StagePanel::CheckUpdateVisitor::
CheckUpdateVisitor(int version) 
	: m_version(version)
	, m_update(false) 
{}

void StagePanel::CheckUpdateVisitor::
Visit(d2d::Object* object, bool& bFetchNext)
{
	d2d::ISprite* spr = static_cast<d2d::ISprite*>(object);
	if (spr->Update(m_version)) {
		m_update = true;
		bFetchNext = false;
	} else {
		bFetchNext = true;
	}
}

} // eanim