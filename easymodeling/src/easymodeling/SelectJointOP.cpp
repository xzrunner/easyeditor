
#include "SelectJointOP.h"
#include "DrawUtils.h"
#include "StagePanel.h"
#include "JointPropertySetting.h"
#include "WorldPropertySetting.h"

using namespace emodeling;

SelectJointOP::SelectJointOP(wxWindow* stage_wnd,
							 ee::EditPanelImpl* stage, 
							 ee::MultiSpritesImpl* spritesImpl, 
							 ee::EditCMPT* callback /*= NULL*/)
	: SelectBodyOP(stage_wnd, stage, spritesImpl, callback)
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
		static_cast<StagePanel*>(m_wnd)->removeJoint(m_selected);
		m_selected = NULL;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool SelectJointOP::OnMouseLeftDown(int x, int y)
{
	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	libmodeling::Joint* selected = static_cast<StagePanel*>(m_wnd)->queryJointByPos(pos);
	if (selected && !m_selected || !selected && m_selected)
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	if (selected)
	{
		if (m_stage->GetKeyState(WXK_CONTROL))
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

	m_first_pos = pos;

	return false;
}

bool SelectJointOP::OnMouseLeftUp(int x, int y)
{
	//ee::Vector pos = m_stage->transPosScreenToProject(x, y);
	//selectedJoints.clear();
	//static_cast<StagePanel*>(m_stage)->queryJointsByRect(ee::Rect(pos, m_firstPos), selectedJoints);
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

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	libmodeling::Joint* joint = static_cast<StagePanel*>(m_wnd)->queryJointByPos(pos);
	if (joint && !m_mouseOn || !joint && m_mouseOn)
		ee::SetCanvasDirtySJ::Instance()->SetDirty();

	m_mouseOn = joint;

	return false;
}

bool SelectJointOP::OnMouseDrag(int x, int y)
{
	if (SelectBodyOP::OnMouseDrag(x, y)) 
		return true;

	if (m_selected)
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		switch (m_selected->type)
		{
		case libmodeling::Joint::e_revoluteJoint:
			{
				libmodeling::RevoluteJoint* joint = static_cast<libmodeling::RevoluteJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_prismaticJoint:
			{
				libmodeling::PrismaticJoint* joint = static_cast<libmodeling::PrismaticJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_distanceJoint:
			{
				libmodeling::DistanceJoint* joint = static_cast<libmodeling::DistanceJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_pulleyJoint:
			{
				libmodeling::PulleyJoint* joint = static_cast<libmodeling::PulleyJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
				const float disGA = ee::Math2D::GetDistance(pos, joint->groundAnchorA),
					disGB = ee::Math2D::GetDistance(pos, joint->groundAnchorB);

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
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_weldJoint:
			{
				libmodeling::WeldJoint* joint = static_cast<libmodeling::WeldJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_frictionJoint:
			{
				libmodeling::FrictionJoint* joint = static_cast<libmodeling::FrictionJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_ropeJoint:
			{
				libmodeling::RopeJoint* joint = static_cast<libmodeling::RopeJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->getWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->getWorldAnchorB());
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

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
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
Visit(ee::Object* object, bool& next) 
{
	std::vector<ee::Vector> bound;
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);
	libmodeling::Body* body = static_cast<libmodeling::Body*>(sprite->GetUserData());
	DrawUtils::drawBody(body, DrawUtils::e_selected);
	next = true;
}
