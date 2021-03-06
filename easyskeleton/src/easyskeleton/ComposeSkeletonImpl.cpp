#include "ComposeSkeletonImpl.h"
#include "StagePanel.h"
#include "TranslateSprState.h"
#include "RotateSprState.h"
#include "ScaleSprState.h"
#include "Bone.h"
#include "Joint.h"

#include <ee/EditPanelImpl.h>
#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>
#include <ee/SpriteSelection.h>

namespace eskeleton
{

ComposeSkeletonImpl::ComposeSkeletonImpl(StagePanel* stage, ee::PropertySettingPanel* property)
	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property, ee::ArrangeSpriteConfig(false, true, true, true))
	, m_selected_joint(NULL)
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
 
  	Joint* joint = QueryJoint(pos);
  	if (joint) {
  		m_selected_joint = joint;
  	} else {
 		ee::ArrangeSpriteImpl::OnMouseLeftDown(x, y);
 	}
}

void ComposeSkeletonImpl::OnMouseLeftUp(int x, int y)
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
 			std::vector<ee::SprPtr> sprs;
 			m_selection->Traverse(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
			AutoAbsorb(sprs[0]);
 		}
 	}
}

void ComposeSkeletonImpl::OnMouseRightDown(int x, int y)
{
	ee::ArrangeSpriteImpl::OnMouseRightDown(x, y);
	m_first_pos = m_stage->TransPosScrToProj(x, y);
}

void ComposeSkeletonImpl::OnMouseDrag(int x, int y)
{
 	if (m_selected_joint)
 	{
 		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
 		m_selected_joint->BindSkin(pos, false);
 		ee::SetCanvasDirtySJ::Instance()->SetDirty();
 	}
 	else
 	{
		ee::ArrangeSpriteImpl::OnMouseDrag(x, y);
	}
}

void ComposeSkeletonImpl::OnPopMenuSelected(int type)
{
	ee::ArrangeSpriteImpl::OnPopMenuSelected(type);

	switch(type)
	{
	case StagePanel::Menu_AddJointNode:
		{
			auto spr = m_sprites_impl->QuerySpriteByPos(m_first_pos);
			if (spr) {
				Bone* bone = (Bone*)(spr->GetUserData());
				s2::JointPose src(spr->GetCenter(), spr->GetAngle(), spr->GetScale()),
					          dst(m_first_pos, spr->GetAngle(), spr->GetScale());
				s2::JointPose pose = s2::world2local(src, dst);
				bone->AddJoint(new Joint(spr, pose));
			}
		}
		break;
	case StagePanel::Menu_DelJointNode:
		{
			auto spr = m_sprites_impl->QuerySpriteByPos(m_first_pos);
			if (spr) {
				Bone* bone = (Bone*)(spr->GetUserData());
				Joint* joint = bone->QueryJoint(m_first_pos);
				bone->RemoveJoint(joint);
			}
		}
		break;
	}
}

void ComposeSkeletonImpl::OnDirectionKeyDown(int type)
{
	ee::ArrangeSpriteImpl::OnDirectionKeyDown(type);
	UpdateSelectedBody();
}

void ComposeSkeletonImpl::OnSpaceKeyDown()
{
	ee::ArrangeSpriteImpl::OnSpaceKeyDown();
	UpdateSelectedBody();
}

void ComposeSkeletonImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	ee::ArrangeSpriteImpl::SetRightPopupMenu(menu, x, y);

	if (m_selection->Size() != 1) {
		return;
	}

	std::vector<ee::SprPtr> sprs;
	m_sprites_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	Bone* bone = (Bone*)(sprs[0]->GetUserData());
	Joint* joint = bone->QueryJoint(m_first_pos);
	if (joint) {
		menu.Append(StagePanel::Menu_DelJointNode, "Del Joint");
	} else {
		menu.Append(StagePanel::Menu_AddJointNode, "Add Joint");
	}
}

ee::ArrangeSpriteState* 
ComposeSkeletonImpl::CreateTranslateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
{
	return new TranslateSprState(selection, first_pos);
}

ee::ArrangeSpriteState* 
ComposeSkeletonImpl::CreateRotateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
{
	return new RotateSprState(selection, first_pos);
}

ee::ArrangeSpriteState* 
ComposeSkeletonImpl::CreateScaleState(const ee::SprPtr& spr, const ee::SpriteCtrlNode::Node& ctrl_node) const
{
	return new ScaleSprState(spr, ctrl_node);
}

Joint* ComposeSkeletonImpl::QueryJoint(const sm::vec2& pos) const
{
	std::vector<ee::SprPtr> sprs;
	m_sprites_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		Bone* bone = (Bone*)(sprs[i]->GetUserData());
		if (Joint* joint = bone->QueryJoint(pos)) {
			return joint;
		}
	}
	return NULL;
}

void ComposeSkeletonImpl::AutoAbsorb(const ee::SprPtr& spr) const
{
	Bone* src = (Bone*)(spr->GetUserData());
	std::vector<ee::SprPtr> sprs;
	m_sprites_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		if (sprs[i] == spr) {
			continue;
		}
		Bone* dst = (Bone*)(sprs[i]->GetUserData());
		if (src->AutoAbsorb(dst)) {
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			break;
		}
	}
}

void ComposeSkeletonImpl::UpdateSelectedBody()
{
	std::vector<ee::SprPtr> sprs;
	m_sprites_impl->TraverseSprites(ee::FetchAllRefVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		Bone* bone = (Bone*)(sprs[i]->GetUserData());
		bone->Update();
	}
}

}