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
#include <ee/CameraCanvas.h>

#include <sprite2/OrthoCamera.h>

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

bool SkeletonImpl::OnKeyDown(int keyCode)
{
	bool ret = ee::ArrangeSpriteImpl::OnKeyDown(keyCode);
	if (KeyDownHandler::Instance()->Process(keyCode)) {
		ret = true;
	}
	return ret;
}

void SkeletonImpl::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
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
			std::vector<ee::SprPtr> sprs;
			m_selection->Traverse(ee::FetchAllRefVisitor<ee::Sprite>(sprs));

			SkeletonData* skeleton = get_curr_skeleton();
			if (skeleton) {
				skeleton->Absorb(sprs[0]);
				skeleton->UpdateJoint(sprs[0]);
				ee::SetCanvasDirtySJ::Instance()->SetDirty();
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
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
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
				auto spr = ViewMgr::Instance()->stage->QuerySpriteByPos(m_first_pos);
				skeleton->InsertJoint(spr, m_first_pos);
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

void SkeletonImpl::OnDraw(float cam_scale) const
{
	ee::ArrangeSpriteImpl::OnDraw(cam_scale);

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
	if (joint) {
		menu.Append(StagePanel::Menu_DelJointNode, "Del Joint");
	} else {
		menu.Append(StagePanel::Menu_AddJointNode, "Add Joint");
	}
}

ee::ArrangeSpriteState* 
SkeletonImpl::CreateTranslateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
{
	return new TranslateSpriteState(selection, first_pos);
}

ee::ArrangeSpriteState* 
SkeletonImpl::CreateRotateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
{
	return new RotateSpriteState(selection, first_pos);
}

}