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
	, m_key_handler(editop, stage->GetController())
	, m_selected_joint(NULL)
{
}

void SkeletonImpl::OnKeyDown(int keyCode)
{
	d2d::ArrangeSpriteImpl::OnKeyDown(keyCode);
	m_key_handler.process(keyCode);
}

void SkeletonImpl::OnMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos = pos;
	Joint* joint = m_stage->getSkeletonData().queryJointByPos(pos);
	if (joint) {
		m_selected_joint = joint;
	} else {
		d2d::ArrangeSpriteImpl::OnMouseLeftDown(x, y);
	}
}

void SkeletonImpl::OnMouseLeftUp(int x, int y)
{
	if (m_selected_joint) 
	{
		m_selected_joint = NULL;
	}
	else
	{
		d2d::ArrangeSpriteImpl::OnMouseLeftUp(x, y);

		if (m_selection->Size() == 1)
		{
			std::vector<d2d::ISprite*> sprites;
			m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
			m_stage->getSkeletonData().absorb(sprites[0]);				
		}
	}
}

void SkeletonImpl::OnMouseRightDown(int x, int y)
{
	d2d::ArrangeSpriteImpl::OnMouseRightDown(x, y);
	m_first_pos = m_stage->TransPosScrToProj(x, y);
}

void SkeletonImpl::OnMouseDrag(int x, int y)
{
	if (m_selected_joint)
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_selected_joint->setPosition(pos);
		m_stage->Refresh();
	}
	else
	{
		d2d::ArrangeSpriteImpl::OnMouseDrag(x, y);
	}
}

void SkeletonImpl::OnPopMenuSelected(int type)
{
	d2d::ArrangeSpriteImpl::OnPopMenuSelected(type);

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

void SkeletonImpl::OnDraw() const
{
	d2d::ArrangeSpriteImpl::OnDraw(*m_stage->GetCamera());
	m_stage->getSkeletonData().draw();
}

void SkeletonImpl::SetRightPopupMenu(wxMenu& menu, d2d::ISprite* spr)
{
	d2d::ArrangeSpriteImpl::SetRightPopupMenu(menu, spr);
	Joint* joint = m_stage->getSkeletonData().queryJointByPos(m_first_pos);
	if (joint)
		menu.Append(StagePanel::Menu_DelJointNode, "Add Joint");
	else
		menu.Append(StagePanel::Menu_AddJointNode, "Del Joint");
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