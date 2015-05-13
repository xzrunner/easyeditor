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

std::string StagePanel::menu_entries[] = 
{
	"Add Joint",
	"Del Joint",
};

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::PropertySettingPanel* property,
					   Controller* ctrl)
	: EditPanel(parent, frame)
	, MultiSpritesImpl(parent)
	, m_ctrl(ctrl)
{
//	m_editOP = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(this, this);
	m_editOP = new ArrangeSpriteOP(this, property, ctrl);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new d2d::StageDropTarget(this, this, m_ctrl->GetLibraryPanel()));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::onMenuAddJointNode, this, Menu_AddJointNode);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &StagePanel::onMenuDelJointNode, this, Menu_DelJointNode);
}

StagePanel::~StagePanel()
{
	clear();
}

void StagePanel::clear()
{
	EditPanel::clear();
	clearSprites();
}

void StagePanel::traverseSprites(d2d::IVisitor& visitor, 
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
		if ((type == d2d::DT_EDITABLE && !layer->editable) ||
			(type == d2d::DT_VISIBLE && !layer->visible))
			continue;

		KeyFrame* frame = layer->getCurrKeyFrame(m_ctrl->frame());
		if (!frame) continue;

		if (order)
		{
			for (int i = 0, n = frame->size(); i < n; ++i) {
				bool hasNext;
				visitor.visit(const_cast<d2d::ISprite*>(frame->getSprite(i)), hasNext);
				if (!hasNext) break;
			}
		}
		else
		{
			for (int i = frame->size() - 1; i >= 0; --i) {
				bool hasNext;
				visitor.visit(const_cast<d2d::ISprite*>(frame->getSprite(i)), hasNext);
				if (!hasNext) break;
			}
		}
	}
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	KeyFrame* frame = m_ctrl->getCurrFrame();
	bool success = frame->remove(sprite);
	if (success) {
		m_ctrl->Refresh();
	}
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	SpriteUserData* ud = (SpriteUserData*)sprite->getUserData();
 	int old_layer = m_ctrl->layer(),
 		old_frame = m_ctrl->frame();
	if (ud) {
		ud->frame = old_frame;
		m_ctrl->setCurrFrame(ud->layer, old_frame);
	}

	KeyFrame* frame = m_ctrl->getCurrFrame();
	frame->insert(sprite);
	Refresh();

	if (ud) {
		m_ctrl->setCurrFrame(old_layer, old_frame);
	}

	m_ctrl->Refresh();
}

void StagePanel::clearSprites()
{
	m_ctrl->ClearLayers();
	m_ctrl->setCurrFrame(-1, -1);

// 	Context* context = Context::Instance();
// 	KeyFrame* frame = context->layers.getLayer(context->currLayer)->getCurrKeyFrame(context->currFrame);
// 	frame->clear();
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	KeyFrame* frame = m_ctrl->getCurrFrame();
	frame->reorder(sprite, up);
}

SkeletonData& StagePanel::getSkeletonData()
{
	KeyFrame* frame = m_ctrl->getCurrFrame();
	return frame->getSkeletonData();	
}

void StagePanel::onMenuAddJointNode(wxCommandEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_AddJointNode);
	Refresh();
}

void StagePanel::onMenuDelJointNode(wxCommandEvent& event)
{
	m_editOP->onPopMenuSelected(Menu_DelJointNode);
	Refresh();
}

} // eanim