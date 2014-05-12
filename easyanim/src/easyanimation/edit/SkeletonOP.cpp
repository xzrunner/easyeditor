#include "SkeletonOP.h"

#include "dataset/Joint.h"
#include "dataset/SkeletonData.h"
#include "view/StagePanel.h"
#include "frame/Context.h"

namespace eanim
{

SkeletonOP::SkeletonOP(StagePanel* stage)
	: d2d::ArrangeSpriteOP<SelectSpritesOP>(stage, stage, 
	static_cast<d2d::PropertySettingPanel*>(Context::Instance()->property))
	, m_keyDownHandler(this)
	, m_stage(stage)
	, m_selectedJoint(NULL)
{
}

bool SkeletonOP::onKeyDown(int keyCode)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onKeyDown(keyCode)) 
		return true;

	m_keyDownHandler.process(keyCode);

	return false;
}

bool SkeletonOP::onMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	Joint* joint = m_stage->getSkeletonData().queryJointByPos(pos);
	if (joint)
	{
		m_selectedJoint = joint;
		return false;
	}
	else
	{
		return d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseLeftDown(x, y);
	}
}

bool SkeletonOP::onMouseLeftUp(int x, int y)
{
	if (m_selectedJoint) 
	{
		m_selectedJoint = NULL;
		return false;
	}
	else
	{
		if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseLeftUp(x, y))
			return true;

		if (m_selection->size() == 1)
		{
			std::vector<d2d::ISprite*> sprites;
			m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
			m_stage->getSkeletonData().absorb(sprites[0]);				
		}

		return false;
	}
}

bool SkeletonOP::onMouseDrag(int x, int y)
{
	if (m_selectedJoint)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		m_selectedJoint->setPosition(pos);
		m_editPanel->Refresh();
		return false;
	}
	else
	{
		return d2d::ArrangeSpriteOP<SelectSpritesOP>::onMouseDrag(x, y);
	}
}

bool SkeletonOP::onPopMenuSelected(int type)
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onPopMenuSelected(type)) 
		return true;

	switch(type)
	{
	case StagePanel::Menu_AddJointNode:
		{
			d2d::ISprite* sprite = m_stage->querySpriteByPos(m_firstPos);
			m_stage->getSkeletonData().insertJoint(sprite, m_firstPos);
		}
		break;
	case StagePanel::Menu_DelJointNode:
		m_stage->getSkeletonData().removeJoint(m_firstPos);
		break;
	}

	return false;
}

bool SkeletonOP::onDraw() const
{
	if (d2d::ArrangeSpriteOP<SelectSpritesOP>::onDraw()) 
		return true;

	StagePanel* panel = static_cast<StagePanel*>(m_editPanel);
	panel->getSkeletonData().draw();

	return false;
}

void SkeletonOP::translateSprite(const d2d::Vector& delta)
{
	d2d::ArrangeSpriteOP<SelectSpritesOP>::translateSprite(delta);

	if (!m_selection->empty()) 
	{
		std::vector<d2d::ISprite*> sprites;
		m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
		m_stage->getSkeletonData().updateJoint(sprites[0]);
	}
}

void SkeletonOP::rotateSprite(const d2d::Vector& dst)
{
	d2d::ArrangeSpriteOP<SelectSpritesOP>::rotateSprite(dst);

	if (!m_selection->empty()) 
	{
		std::vector<d2d::ISprite*> sprites;
		m_selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
		m_stage->getSkeletonData().fixJoint(sprites[0]);
		float dAngle = d2d::Math::getAngleInDirection(sprites[0]->getPosition(), m_lastPos, dst);
		m_stage->getSkeletonData().updateJoint(sprites[0], dAngle);
	}
}

void SkeletonOP::scaleSprite(const d2d::Vector& currPos)
{
	d2d::ArrangeSpriteOP<SelectSpritesOP>::scaleSprite(currPos);
}

void SkeletonOP::setRightPopupMenu(wxMenu& menu)
{
	d2d::ArrangeSpriteOP<SelectSpritesOP>::setRightPopupMenu(menu);

	Joint* joint = m_stage->getSkeletonData().queryJointByPos(m_firstPos);
	if (joint)
		menu.Append(StagePanel::Menu_DelJointNode, StagePanel::menu_entries[StagePanel::Menu_DelJointNode - d2d::EditPanel::Menu_End]);
	else
		menu.Append(StagePanel::Menu_AddJointNode, StagePanel::menu_entries[StagePanel::Menu_AddJointNode - d2d::EditPanel::Menu_End]);
}

}