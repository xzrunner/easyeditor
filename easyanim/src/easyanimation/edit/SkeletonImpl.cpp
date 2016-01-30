#include "SkeletonImpl.h"
#include "KeyDownHandler.h"

#include "TranslateSpriteState.h"
#include "RotateSpriteState.h"

#include "dataset/Joint.h"
#include "dataset/SkeletonData.h"
#include "dataset/data_utility.h"
#include "view/StagePanel.h"
#include "view/ViewMgr.h"

namespace eanim
{

SkeletonImpl::SkeletonImpl()
	: d2d::ArrangeSpriteImpl(ViewMgr::Instance()->stage, 
	                         ViewMgr::Instance()->stage->GetStageImpl(), 
							 ViewMgr::Instance()->stage, 
							 ViewMgr::Instance()->property, 
							 d2d::ArrangeSpriteConfig())
	, m_selected_joint(NULL)
{
}

void SkeletonImpl::OnKeyDown(int keyCode)
{
	d2d::ArrangeSpriteImpl::OnKeyDown(keyCode);
	KeyDownHandler::Instance()->Process(keyCode);
}

void SkeletonImpl::OnMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	m_first_pos = pos;

	SkeletonData* skeleton = get_curr_skeleton();
	if (!skeleton) {
		return;
	}

	Joint* joint = skeleton->QueryJointByPos(pos);
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
			std::vector<d2d::Sprite*> sprites;
			m_selection->Traverse(d2d::FetchAllVisitor<d2d::Sprite>(sprites));

			SkeletonData* skeleton = get_curr_skeleton();
			if (skeleton) {
				skeleton->Absorb(sprites[0]);
			}
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
		m_selected_joint->SetPosition(pos);
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
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
			SkeletonData* skeleton = get_curr_skeleton();
			if (skeleton) {
				d2d::Sprite* sprite = ViewMgr::Instance()->stage->QuerySpriteByPos(m_first_pos);
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
	d2d::ArrangeSpriteImpl::OnDraw(*m_stage->GetCamera());

	SkeletonData* skeleton = get_curr_skeleton();
	if (skeleton) {
		skeleton->Draw();
	}
}

void SkeletonImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	d2d::ArrangeSpriteImpl::SetRightPopupMenu(menu, x, y);

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

d2d::IArrangeSpriteState* 
SkeletonImpl::CreateTranslateState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const
{
	return new TranslateSpriteState(selection, first_pos);
}

d2d::IArrangeSpriteState* 
SkeletonImpl::CreateRotateState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const
{
	return new RotateSpriteState(selection, first_pos);
}

}