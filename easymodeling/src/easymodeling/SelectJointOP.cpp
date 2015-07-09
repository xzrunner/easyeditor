
#include "SelectJointOP.h"
#include "DrawUtils.h"
#include "StagePanel.h"
#include "JointPropertySetting.h"
#include "WorldPropertySetting.h"

using namespace emodeling;

SelectJointOP::SelectJointOP(d2d::EditPanel* editPanel, 
							 d2d::MultiSpritesImpl* spritesImpl, 
							 d2d::ViewPanelMgr* view_panel_mgr,
							 d2d::AbstractEditCMPT* callback /*= NULL*/)
	: SelectBodyOP(editPanel, spritesImpl, view_panel_mgr, callback)
	, m_property_panel(NULL)
	, m_mouseOn(NULL)
	, m_selected(NULL)
{
}

bool SelectJointOP::OnKeyDown(int keyCode)
{
	if (SelectBodyOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE && m_selected)
	{
		if (m_mouseOn == m_selected) m_mouseOn = NULL;
		static_cast<StagePanel*>(m_stage)->removeJoint(m_selected);
		m_selected = NULL;
		m_stage->Refresh();
	}

	return false;
}

bool SelectJointOP::OnMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	libmodeling::Joint* selected = static_cast<StagePanel*>(m_stage)->queryJointByPos(pos);
	if (selected && !m_selected || !selected && m_selected)
		m_stage->Refresh();
	if (selected)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (jointSelection.IsExist(selected))
				jointSelection.Remove(selected);
			else
				jointSelection.Add(selected);
		}
		else
		{
			if (!jointSelection.IsExist(selected))
			{
				jointSelection.Clear();
				jointSelection.Add(selected);
			}
		}

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		m_selection->Clear();
	}

	m_selected = selected;

	if (m_selected)
	{
		m_property_panel->SetPropertySetting(new JointPropertySetting(m_stage, m_selected));
//		m_selection->clear();
	}
	else
	{
		m_property_panel->SetPropertySetting(new WorldPropertySetting(m_stage));
		SelectBodyOP::OnMouseLeftDown(x, y);
	}

	m_firstPos = pos;

	return false;
}

bool SelectJointOP::OnMouseLeftUp(int x, int y)
{
	//d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
	//selectedJoints.clear();
	//static_cast<StagePanel*>(m_stage)->queryJointsByRect(d2d::Rect(pos, m_firstPos), selectedJoints);
	//if (selectedJoints.size() == 1)
	//	m_property_panel->SetPropertySetting(new JointPropertySetting(m_stage, selectedJoints[0]));

	if (SelectBodyOP::OnMouseLeftUp(x, y)) return true;

	if (m_selected)
		m_property_panel->SetPropertySetting(new JointPropertySetting(m_stage, m_selected));
	else
		m_property_panel->SetPropertySetting(new WorldPropertySetting(m_stage));

	return false;
}

bool SelectJointOP::OnMouseMove(int x, int y)
{
	if (SelectBodyOP::OnMouseMove(x, y)) 
		return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	libmodeling::Joint* joint = static_cast<StagePanel*>(m_stage)->queryJointByPos(pos);
	if (joint && !m_mouseOn || !joint && m_mouseOn)
		m_stage->Refresh();

	m_mouseOn = joint;

	return false;
}

bool SelectJointOP::OnMouseDrag(int x, int y)
{
	if (SelectBodyOP::OnMouseDrag(x, y)) 
		return true;

	if (m_selected)
	{
		d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
		switch (m_selected->type)
		{
		case libmodeling::Joint::e_revoluteJoint:
			{
				libmodeling::RevoluteJoint* joint = static_cast<libmodeling::RevoluteJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_prismaticJoint:
			{
				libmodeling::PrismaticJoint* joint = static_cast<libmodeling::PrismaticJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_distanceJoint:
			{
				libmodeling::DistanceJoint* joint = static_cast<libmodeling::DistanceJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_pulleyJoint:
			{
				libmodeling::PulleyJoint* joint = static_cast<libmodeling::PulleyJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				const float disGA = d2d::Math::getDistance(pos, joint->groundAnchorA),
					disGB = d2d::Math::getDistance(pos, joint->groundAnchorB);

				float dis = std::min(std::min(disA, disB), std::min(disGA, disGB));
				if (dis == disA)
					joint->setLocalAnchorA(pos);
				else if (dis == disB)
					joint->setLocalAnchorB(pos);
				else if (dis == disGA)
					joint->groundAnchorA = pos;
				else
					joint->groundAnchorB = pos;					
			}
			break;
		case libmodeling::Joint::e_gearJoint:
			{
			}
			break;
		case libmodeling::Joint::e_wheelJoint:
			{
				libmodeling::WheelJoint* joint = static_cast<libmodeling::WheelJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_weldJoint:
			{
				libmodeling::WeldJoint* joint = static_cast<libmodeling::WeldJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_frictionJoint:
			{
				libmodeling::FrictionJoint* joint = static_cast<libmodeling::FrictionJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_ropeJoint:
			{
				libmodeling::RopeJoint* joint = static_cast<libmodeling::RopeJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_motorJoint:
			{
			}
			break;
		}

		m_stage->Refresh();
	}

	return false;
}

bool SelectJointOP::OnDraw() const
{
	if (SelectBodyOP::OnDraw()) 
		return true;

	m_selection->Traverse(DrawSelectedVisitor());

	if (m_mouseOn)
		m_mouseOn->draw(libmodeling::Joint::e_mouseOn);
	if (m_selected) 
		m_selected->draw(libmodeling::Joint::e_selected);

	return false;
}

bool SelectJointOP::Clear()
{
	if (SelectBodyOP::Clear()) 
		return true;

	m_selected = m_mouseOn = NULL;

	return false;
}

void SelectJointOP::DrawSelectedVisitor::
Visit(d2d::Object* object, bool& bFetchNext) 
{
	std::vector<d2d::Vector> bound;
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	libmodeling::Body* body = static_cast<libmodeling::Body*>(sprite->GetUserData());
	DrawUtils::drawBody(body, DrawUtils::e_selected);
	bFetchNext = true;
}
