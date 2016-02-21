#include "SkeletonImpl.h"
#include "KeyDownHandler.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"

#include "dataset/Joint.h"
#include "dataset/SkeletonData.h"
#include "dataset/data_utility.h"
#include "view/StagePanel.h"
#include "view/ViewMgr.h"

#include <ee/EditPanelImpl.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>

namespace eanim
{

SkeletonImpl::SkeletonImpl()
	: ee::ArrangeSpriteImpl(ViewMgr::Instance()->stage, 
	                         ViewMgr::Instance()->stage->GetStageImpl(), 
							 ViewMgr::Instance()->stage, 
							 ViewMgr::Instance()->property, 
							 ee::ArrangeSpriteConfig())
	, m_selected_joint(NULL)
{
}

void SkeletonImpl::OnKeyDown(int keyCode)
{
	ee::ArrangeSpriteImpl::OnKeyDown(keyCode);
	KeyDownHandler::Instance()->Process(keyCode);
}

void SkeletonImpl::OnMouseLeftDown(int x, int y)
{
	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos = pos;

	SkeletonData* skeleton = get_curr_skeleton();
	if (!skeleton) {
		return;
	}

	Joint* joint = skeleton->QueryJointByPos(pos);
	if (joint) {
		m_selected_joint = joint;
	} else {
		ee::ArrangeSpriteImpl::OnMouseLeftDown(x, y);
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
		ee::ArrangeSpriteImpl::OnMouseLeftUp(x, y);

		if (m_selection->Size() == 1)
		{
			std::vector<ee::Sprite*> sprites;
			m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));

			SkeletonData* skeleton = get_curr_skeleton();
			if (skeleton) {
				skeleton->Absorb(sprites[0]);
			}
		}
	}
}

void SkeletonImpl::OnMouseRightDown(int x, int y)
{
	ee::ArrangeSpriteImpl::OnMouseRightDown(x, y);
	m_first_pos = m_stage->TransPosScrToProj(x, y);
}

void SkeletonImpl::OnMouseDrag(int x, int y)
{
	if (m_selected_joint)
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_selected_joint->SetPosition(pos);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
	else
	{
		ee::ArrangeSpriteImpl::OnMouseDrag(x, y);
	}
}

void SkeletonImpl::OnPopMenuSelected(int type)
{
	ee::ArrangeSpriteImpl::OnPopMenuSelected(type);

	switch(type)
	{
	case StagePanel::Menu_AddJointNode:
		{
			SkeletonData* skeleton = get_curr_skeleton();
			if (skeleton) {
				ee::Sprite* sprite = ViewMgr::Instance()->stage->QuerySpriteByPos(m_first_pos);
				skeleton->InsertJoint(sprite, m_first_pos);
			}
		}
		break;
	case StagePanel::Menu_DelJointNode:
		{
			SkeletonData* skeleton = get_curr_skeleton();
			if (skeleton) {
				skeleton->RemoveJoint(m_first_pos);
			}
		}
		break;
	}
}

void SkeletonImpl::OnDraw() const
{
	ee::ArrangeSpriteImpl::OnDraw(*m_stage->GetCamera());

	SkeletonData* skeleton = get_curr_skeleton();
	if (skeleton) {
		skeleton->Draw();
	}
}

void SkeletonImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	ee::ArrangeSpriteImpl::SetRightPopupMenu(menu, x, y);

	SkeletonData* skeleton = get_curr_skeleton();
	if (!skeleton) {
		return;
	}

	Joint* joint = skeleton->QueryJointByPos(m_first_pos);
	if (joint)
		menu.Append(StagePanel::Menu_DelJointNode, "Add Joint");
	else
		menu.Append(StagePanel::Menu_AddJointNode, "Del Joint");
}

ee::ArrangeSpriteState* 
SkeletonImpl::CreateTranslateState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const
{
	return new TranslateSpriteState(selection, first_pos);
}

ee::ArrangeSpriteState* 
SkeletonImpl::CreateRotateState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const
{
	return new RotateSpriteState(selection, first_pos);
}

}