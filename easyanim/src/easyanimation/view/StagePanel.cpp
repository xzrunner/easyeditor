#include "StagePanel.h"
#include "StageCanvas.h"
#include "KeysPanel.h"

#include "frame/Controller.h"
#include "edit/ArrangeSpriteOP.h"
#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"
#include "dataset/SpriteUserData.h"

namespace eanim
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::PropertySettingPanel* property,
					   Controller* ctrl)
	: EditPanel(parent, frame)
	, MultiSpritesImpl(GetStageImpl())
	, m_ctrl(ctrl)
{
//	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this);
	SetEditOP(new ArrangeSpriteOP(this, property, ctrl));
	SetCanvas(new StageCanvas(this));

	SetDropTarget(new d2d::StageDropTarget(this, GetStageImpl(), m_ctrl->GetLibraryPanel()));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::onMenuAddJointNode, this, Menu_AddJointNode);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::onMenuDelJointNode, this, Menu_DelJointNode);

	m_subjects.push_back(d2d::ReorderSpriteSJ::Instance());
	m_subjects.push_back(d2d::ReorderSpriteMostSJ::Instance());
	m_subjects.push_back(d2d::InsertSpriteSJ::Instance());
	m_subjects.push_back(d2d::RemoveSpriteSJ::Instance());
	m_subjects.push_back(d2d::ClearSpriteSJ::Instance());
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

StagePanel::~StagePanel()
{
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void StagePanel::Notify(int sj_id, void* ud)
{
	MultiSpritesImpl::Notify(sj_id, ud);
	
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
			bool ret = m_ctrl->ClearAllLayer();
			m_ctrl->setCurrFrame(-1, -1);

			if (ret) {
				GetCanvas()->SetDirty();
			}

			// 	Context* context = Context::Instance();
			// 	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
			// 	frame->clear();
		}
		break;
	}
}

void StagePanel::TraverseSprites(d2d::IVisitor& visitor, 
								 d2d::DataTraverseType type/* = d2d::e_allExisting*/,
								 bool order/* = true*/) const
{
	if (m_ctrl->layer() == -1 || m_ctrl->frame() == -1) {
		return;
	}

	const std::vector<Layer*>& layers = m_ctrl->GetLayers().getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		Layer* layer = layers[i];
		if ((type == d2d::DT_EDITABLE && !layer->IsEditable()) ||
			(type == d2d::DT_VISIBLE && !layer->IsVisible()))
			continue;

		KeyFrame* frame = layer->GetCurrKeyFrame(m_ctrl->frame());
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

SkeletonData& StagePanel::getSkeletonData()
{
	KeyFrame* frame = m_ctrl->getCurrFrame();
	return frame->GetSkeletonData();	
}

void StagePanel::onMenuAddJointNode(wxCommandEvent& event)
{
	GetEditOP()->OnPopMenuSelected(Menu_AddJointNode);
	SetCanvasDirty();
}

void StagePanel::onMenuDelJointNode(wxCommandEvent& event)
{
	GetEditOP()->OnPopMenuSelected(Menu_DelJointNode);
	SetCanvasDirty();
}

void StagePanel::Reorder(d2d::ISprite* spr, bool up)
{
	KeyFrame* frame = m_ctrl->getCurrFrame();
	bool succ = frame->Reorder(spr, up);
	if (succ) {
		SetCanvasDirty();
	}
}

void StagePanel::ReorderMost(d2d::ISprite* spr, bool up)
{
	KeyFrame* frame = m_ctrl->getCurrFrame();
	bool succ = frame->ReorderMost(spr, up);
	if (succ) {
		SetCanvasDirty();
	}
}

void StagePanel::Insert(d2d::ISprite* spr)
{
	SpriteUserData* ud = (SpriteUserData*)spr->GetUserData();
	int old_layer = m_ctrl->layer(),
		old_frame = m_ctrl->frame();
	if (ud) {
		ud->frame = old_frame;
		m_ctrl->setCurrFrame(ud->layer, old_frame);
	}

	KeyFrame* frame = m_ctrl->getCurrFrame();
	assert(frame);
	frame->Insert(spr);

	if (ud) {
		m_ctrl->setCurrFrame(old_layer, old_frame);
	}

	m_ctrl->Refresh();

	SetCanvasDirty();
}

void StagePanel::Remove(d2d::ISprite* spr)
{
	KeyFrame* frame = m_ctrl->getCurrFrame();
	bool success = frame->Remove(spr);
	if (success) {
		m_ctrl->Refresh();
		GetCanvas()->SetDirty();
	}
}

} // eanim