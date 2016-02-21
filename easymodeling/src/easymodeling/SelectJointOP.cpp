#include "SelectJointOP.h"
#include "DrawUtils.h"
#include "StagePanel.h"
#include "JointPropertySetting.h"
#include "WorldPropertySetting.h"

#include <ee/panel_msg.h>
#include <ee/EditPanelImpl.h>
#include <ee/EditCMPT.h>
#include <ee/SpriteSelection.h>
#include <ee/PropertySettingPanel.h>
#include <ee/Math2D.h>

namespace emodeling
{

SelectJointOP::SelectJointOP(wxWindow* stage_wnd,
							 ee::EditPanelImpl* stage, 
							 ee::MultiSpritesImpl* sprites_impl, 
							 ee::EditCMPT* callback /*= NULL*/)
	: SelectBodyOP(stage_wnd, stage, sprites_impl, callback)
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
	Joint* selected = static_cast<StagePanel*>(m_wnd)->queryJointByPos(pos);
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
			m_callback->UpdateControlValue();
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
	Joint* joint = static_cast<StagePanel*>(m_wnd)->queryJointByPos(pos);
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
		switch (m_selected->m_type)
		{
		case Joint::e_revoluteJoint:
			{
				RevoluteJoint* joint = static_cast<RevoluteJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				if (disA < disB)
					joint->SetLocalAnchorA(pos);
				else
					joint->SetLocalAnchorB(pos);
			}
			break;
		case Joint::e_prismaticJoint:
			{
				PrismaticJoint* joint = static_cast<PrismaticJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				if (disA < disB)
					joint->SetLocalAnchorA(pos);
				else
					joint->SetLocalAnchorB(pos);
			}
			break;
		case Joint::e_distanceJoint:
			{
				DistanceJoint* joint = static_cast<DistanceJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				if (disA < disB)
					joint->SetLocalAnchorA(pos);
				else
					joint->SetLocalAnchorB(pos);
			}
			break;
		case Joint::e_pulleyJoint:
			{
				PulleyJoint* joint = static_cast<PulleyJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				const float disGA = ee::Math2D::GetDistance(pos, joint->m_ground_anchor_a),
					disGB = ee::Math2D::GetDistance(pos, joint->m_ground_anchor_b);

				float dis = std::min(std::min(disA, disB), std::min(disGA, disGB));
				if (dis == disA)
					joint->SetLocalAnchorA(pos);
				else if (dis == disB)
					joint->SetLocalAnchorB(pos);
				else if (dis == disGA)
					joint->m_ground_anchor_a = pos;
				else
					joint->m_ground_anchor_b = pos;					
			}
			break;
		case Joint::e_gearJoint:
			{
			}
			break;
		case Joint::e_wheelJoint:
			{
				WheelJoint* joint = static_cast<WheelJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				if (disA < disB)
					joint->SetLocalAnchorA(pos);
				else
					joint->SetLocalAnchorB(pos);
			}
			break;
		case Joint::e_weldJoint:
			{
				WeldJoint* joint = static_cast<WeldJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				if (disA < disB)
					joint->SetLocalAnchorA(pos);
				else
					joint->SetLocalAnchorB(pos);
			}
			break;
		case Joint::e_frictionJoint:
			{
				FrictionJoint* joint = static_cast<FrictionJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				if (disA < disB)
					joint->SetLocalAnchorA(pos);
				else
					joint->SetLocalAnchorB(pos);
			}
			break;
		case Joint::e_ropeJoint:
			{
				RopeJoint* joint = static_cast<RopeJoint*>(m_selected);
				const float disA = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorA()),
					disB = ee::Math2D::GetDistance(pos, joint->GetWorldAnchorB());
				if (disA < disB)
					joint->SetLocalAnchorA(pos);
				else
					joint->SetLocalAnchorB(pos);
			}
			break;
		case Joint::e_motorJoint:
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
		m_mouseOn->Draw(Joint::e_mouseOn);
	if (m_selected) 
		m_selected->Draw(Joint::e_selected);

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
	Body* body = static_cast<Body*>(sprite->GetUserData());
	DrawUtils::DrawBody(body, DrawUtils::e_selected);
	next = true;
}

}