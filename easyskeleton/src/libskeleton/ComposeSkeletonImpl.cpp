#include "ComposeSkeletonImpl.h"
#include "StagePanel.h"

#include <ee/EditPanelImpl.h>
#include <ee/CameraMgr.h>

namespace eskeleton
{

ComposeSkeletonImpl::ComposeSkeletonImpl(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property)
{
			
}

bool ComposeSkeletonImpl::OnKeyDown(int keyCode)
{
	bool ret = ee::ArrangeSpriteImpl::OnKeyDown(keyCode);
// 	if (KeyDownHandler::Instance()->Process(keyCode)) {
// 		ret = true;
// 	}
	return ret;
}

void ComposeSkeletonImpl::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos = pos;

// 	SkeletonData* skeleton = get_curr_skeleton();
// 	if (!skeleton) {
// 		return;
// 	}
// 
// 	Joint* joint = skeleton->QueryJointByPos(pos);
// 	if (joint) {
// 		m_selected_joint = joint;
// 	} else {
 		ee::ArrangeSpriteImpl::OnMouseLeftDown(x, y);
// 	}
}

void ComposeSkeletonImpl::OnMouseLeftUp(int x, int y)
{
// 	if (m_selected_joint) 
// 	{
// 		m_selected_joint = NULL;
// 	}
// 	else
// 	{
 		ee::ArrangeSpriteImpl::OnMouseLeftUp(x, y);
// 
// 		if (m_selection->Size() == 1)
// 		{
// 			std::vector<ee::Sprite*> sprs;
// 			m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
// 
// 			SkeletonData* skeleton = get_curr_skeleton();
// 			if (skeleton) {
// 				skeleton->Absorb(sprs[0]);
// 				skeleton->UpdateJoint(sprs[0]);
// 				ee::SetCanvasDirtySJ::Instance()->SetDirty();
// 			}
// 		}
// 	}
}

void ComposeSkeletonImpl::OnMouseRightDown(int x, int y)
{
	ee::ArrangeSpriteImpl::OnMouseRightDown(x, y);
	m_first_pos = m_stage->TransPosScrToProj(x, y);
}

void ComposeSkeletonImpl::OnMouseDrag(int x, int y)
{
// 	if (m_selected_joint)
// 	{
// 		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
// 		m_selected_joint->SetPosition(pos);
// 		ee::SetCanvasDirtySJ::Instance()->SetDirty();
// 	}
// 	else
// 	{
		ee::ArrangeSpriteImpl::OnMouseDrag(x, y);
//	}
}

void ComposeSkeletonImpl::OnPopMenuSelected(int type)
{
	ee::ArrangeSpriteImpl::OnPopMenuSelected(type);

// 	switch(type)
// 	{
// 	case StagePanel::Menu_AddJointNode:
// 		{
// 			SkeletonData* skeleton = get_curr_skeleton();
// 			if (skeleton) {
// 				ee::Sprite* spr = ViewMgr::Instance()->stage->QuerySpriteByPos(m_first_pos);
// 				skeleton->InsertJoint(spr, m_first_pos);
// 			}
// 		}
// 		break;
// 	case StagePanel::Menu_DelJointNode:
// 		{
// 			SkeletonData* skeleton = get_curr_skeleton();
// 			if (skeleton) {
// 				skeleton->RemoveJoint(m_first_pos);
// 			}
// 		}
// 		break;
// 	}
}

void ComposeSkeletonImpl::OnDraw(const ee::Camera& cam) const
{
	ee::ArrangeSpriteImpl::OnDraw(*ee::CameraMgr::Instance()->GetCamera());

// 	SkeletonData* skeleton = get_curr_skeleton();
// 	if (skeleton) {
// 		skeleton->Draw();
// 	}
}

void ComposeSkeletonImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	ee::ArrangeSpriteImpl::SetRightPopupMenu(menu, x, y);

// 	SkeletonData* skeleton = get_curr_skeleton();
// 	if (!skeleton) {
// 		return;
// 	}
// 
// 	Joint* joint = skeleton->QueryJointByPos(m_first_pos);
// 	if (joint) {
// 		menu.Append(StagePanel::Menu_DelJointNode, "Del Joint");
// 	} else {
// 		menu.Append(StagePanel::Menu_AddJointNode, "Add Joint");
// 	}
}

// ee::ArrangeSpriteState* 
// ComposeSkeletonImpl::CreateTranslateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
// {
// 	return new TranslateSpriteState(selection, first_pos);
// }
// 
// ee::ArrangeSpriteState* 
// ComposeSkeletonImpl::CreateRotateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
// {
// 	return new RotateSpriteState(selection, first_pos);
// }

}