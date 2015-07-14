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
					   d2d::ViewPanelMgr* view_panel_mgr,
					   Controller* ctrl)
	: EditPanel(parent, frame)
	, MultiSpritesImpl(this)
	, m_ctrl(ctrl)
{
//	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this);
	m_edit_op = new ArrangeSpriteOP(this, property, view_panel_mgr, ctrl);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new d2d::StageDropTarget(this, this, m_ctrl->GetLibraryPanel()));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::onMenuAddJointNode, this, Menu_AddJointNode);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::onMenuDelJointNode, this, Menu_DelJointNode);
}

StagePanel::~StagePanel()
{
	Clear();
}

void StagePanel::Clear()
{
	EditPanel::Clear();
	ClearAllSprite();
}

bool StagePanel::ReorderSprite(d2d::ISprite* sprite, bool up)
{
	MultiSpritesImpl::ReorderSprite(sprite, up);

	KeyFrame* frame = m_ctrl->getCurrFrame();
	bool ret = frame->Reorder(sprite, up);
	if (ret) {
		m_canvas->SetDirty();
	}
	return ret;
}

bool StagePanel::InsertSprite(d2d::ISprite* sprite)
{
	MultiSpritesImpl::InsertSprite(sprite);

	SpriteUserData* ud = (SpriteUserData*)sprite->GetUserData();
 	int old_layer = m_ctrl->layer(),
 		old_frame = m_ctrl->frame();
	if (ud) {
		ud->frame = old_frame;
		m_ctrl->setCurrFrame(ud->layer, old_frame);
	}

	KeyFrame* frame = m_ctrl->getCurrFrame();
	assert(frame);
	frame->Insert(sprite);

	if (ud) {
		m_ctrl->setCurrFrame(old_layer, old_frame);
	}

	m_ctrl->Refresh();

	m_canvas->SetDirty();

	return true;
}

bool StagePanel::RemoveSprite(d2d::ISprite* sprite)
{
	MultiSpritesImpl::RemoveSprite(sprite);

	KeyFrame* frame = m_ctrl->getCurrFrame();
	bool success = frame->Remove(sprite);
	if (success) {
		m_ctrl->Refresh();
		m_canvas->SetDirty();
	}
	return success;
}

bool StagePanel::ClearAllSprite()
{
	MultiSpritesImpl::ClearAllSprite();

	bool ret = m_ctrl->ClearAllLayer();
	m_ctrl->setCurrFrame(-1, -1);

	if (ret) {
		m_canvas->SetDirty();
	}

// 	Context* context = Context::Instance();
// 	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
// 	frame->clear();

	return ret;
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
	m_edit_op->OnPopMenuSelected(Menu_AddJointNode);
	SetCanvasDirty();
}

void StagePanel::onMenuDelJointNode(wxCommandEvent& event)
{
	m_edit_op->OnPopMenuSelected(Menu_DelJointNode);
	SetCanvasDirty();
}

} // eanim