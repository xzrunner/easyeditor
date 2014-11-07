#include "SkeletonImpl.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"

#include "dataset/Joint.h"
#include "dataset/SkeletonData.h"
#include "view/StagePanel.h"

namespace eanim
{

SkeletonImpl::SkeletonImpl(StagePanel* stage, d2d::PropertySettingPanel* property,
						   d2d::AbstractEditOP* editop)
						   : d2d::ArrangeSpriteImpl(stage, stage, property, d2d::ArrangeSpriteConfig())
	, m_stage(stage)
	, m_key_handler(editop)
	, m_selected_joint(NULL)
{
}

void SkeletonImpl::onKeyDown(int keyCode)
{
	d2d::ArrangeSpriteImpl::onKeyDown(keyCode);
	m_key_handler.process(keyCode);
}

void SkeletonImpl::onMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	m_first_pos = pos;
	Joint* joint = m_stage->getSkeletonData().queryJointByPos(pos);
	if (joint) {
		m_selected_joint = joint;
	} else {
		d2d::ArrangeSpriteImpl::onMouseLeftDown(x, y);
	}
}

void SkeletonImpl::onMouseLeftUp(int x, int y)
{
	if (m_selected_joint) 
	{
		m_selected_joint = NULL;
	}
	else
	{
		d2d::ArrangeSpriteImpl::onMouseLeftUp(x, y);

		if (m_selection->size() == 1)
		{
			std::vector<d2d::ISprite*> sprites;
			m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
			m_stage->getSkeletonData().absorb(sprites[0]);				
		}
	}
}

void SkeletonImpl::onMouseRightDown(int x, int y)
{
	d2d::ArrangeSpriteImpl::onMouseRightDown(x, y);
	m_first_pos = m_stage->transPosScreenToProject(x, y);
}

void SkeletonImpl::onMouseDrag(int x, int y)
{
	if (m_selected_joint)
	{
		d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
		m_selected_joint->setPosition(pos);
		m_stage->Refresh();
	}
	else
	{
		d2d::ArrangeSpriteImpl::onMouseDrag(x, y);
	}
}

void SkeletonImpl::onPopMenuSelected(int type)
{
	d2d::ArrangeSpriteImpl::onPopMenuSelected(type);

	switch(type)
	{
	case StagePanel::Menu_AddJointNode:
		{
			d2d::ISprite* sprite = m_stage->querySpriteByPos(m_first_pos);
			m_stage->getSkeletonData().insertJoint(sprite, m_first_pos);
		}
		break;
	case StagePanel::Menu_DelJointNode:
		m_stage->getSkeletonData().removeJoint(m_first_pos);
		break;
	}
}

void SkeletonImpl::onDraw() const
{
	d2d::ArrangeSpriteImpl::onDraw(*m_stage->getCamera());
	m_stage->getSkeletonData().draw();
}

void SkeletonImpl::setRightPopupMenu(wxMenu& menu)
{
	d2d::ArrangeSpriteImpl::setRightPopupMenu(menu);
	Joint* joint = m_stage->getSkeletonData().queryJointByPos(m_first_pos);
	if (joint)
		menu.Append(StagePanel::Menu_DelJointNode, StagePanel::menu_entries[StagePanel::Menu_DelJointNode - d2d::EditPanel::Menu_End]);
	else
		menu.Append(StagePanel::Menu_AddJointNode, StagePanel::menu_entries[StagePanel::Menu_AddJointNode - d2d::EditPanel::Menu_End]);
}

d2d::IArrangeSpriteState* 
SkeletonImpl::CreateTransalteState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const
{
	return new TranslateSpriteState(m_stage, selection, first_pos);
}

d2d::IArrangeSpriteState* 
SkeletonImpl::CreateRotateState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const
{
	return new RotateSpriteState(m_stage, selection, first_pos);
}

}