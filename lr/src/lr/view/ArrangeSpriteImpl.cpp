#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "RightPopupMenu.h"

#include "dataset/CharacterAllDirections.h"

#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/Config.h>
#include <ee/SettingData.h>
#include <ee/panel_msg.h>
#include <ee/EditPanelImpl.h>
#include <ee/CameraMgr.h>
#include <ee/Pseudo3DCamera.h>

#include <easycomplex.h>
#include <easyparticle3d.h>

namespace lr
{

ArrangeSpriteImpl::ArrangeSpriteImpl(ee::LibraryPanel* library, 
									 ee::PropertySettingPanel* property,
									 StagePanel* stage, 
									 CharacterAllDirections* dirs)
	: ee::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property)
	, m_popup(library, stage)
	, m_dirs(dirs)
{
}

void ArrangeSpriteImpl::OnKeyDown(int key_code)
{
	ee::ArrangeSpriteImpl::OnKeyDown(key_code);

	if (m_stage->GetKeyState(WXK_SHIFT))
	{
		if (key_code == 'o' || key_code == 'O') {
			ee::CameraMgr::Instance()->SetCamera(ee::CameraMgr::ORTHO);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();			
		} else if (key_code == 'p' || key_code == 'P') {
			ee::CameraMgr::Instance()->SetCamera(ee::CameraMgr::PSEUDO3D);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
		}
	}
	else
	{
		if (ee::CameraMgr::Instance()->IsType(ee::CameraMgr::PSEUDO3D))
		{
			ee::Pseudo3DCamera* cam = static_cast<ee::Pseudo3DCamera*>(ee::CameraMgr::Instance()->GetCamera());
			switch (key_code)
			{
			case WXK_PAGEDOWN:
				cam->Rotate(0.1f);
				break;
			case WXK_PAGEUP:
				cam->Rotate(-0.1f);
				break;
			}
		}
	}
}

void ArrangeSpriteImpl::OnPopMenuSelected(int type)
{
	ee::ArrangeSpriteImpl::OnPopMenuSelected(type);
	m_popup.OnRightPopupMenu(type);
}

void ArrangeSpriteImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	ee::ArrangeSpriteImpl::SetRightPopupMenu(menu, x, y);
	m_popup.SetRightPopupMenu(menu, x, y);
}

ee::ArrangeSpriteState* ArrangeSpriteImpl::
CreateRotateState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const
{
	std::vector<ee::Sprite*> sprites;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	if (sprites.size() == 1 && dynamic_cast<eparticle3d::Sprite*>(sprites[0])) {
		return new ecomplex::SphereRotateState(m_stage, first_pos, static_cast<eparticle3d::Sprite*>(sprites[0])->GetDir());
	} else {
		return new RotateSpriteState(selection, first_pos, m_dirs);
	}
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteImpl::RotateSpriteState
//////////////////////////////////////////////////////////////////////////

ArrangeSpriteImpl::RotateSpriteState::
RotateSpriteState(ee::SpriteSelection* selection, const ee::Vector& first_pos,
				  CharacterAllDirections* dirs)
	: ee::RotateSpriteState(selection, first_pos)
	, m_dirs(dirs)
{
}

void ArrangeSpriteImpl::RotateSpriteState::
Rotate(const ee::Vector& dst)
{
	std::vector<ee::Sprite*> sprs;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];
		std::string filepath = spr->GetSymbol().GetFilepath();
		if (!CharacterFileName::IsValidFilepath(filepath)) {
			ee::Vector center = spr->GetPosition() + spr->GetOffset();
			float angle = ee::Math2D::GetAngleInDirection(center, m_last_pos, dst);
			spr->Rotate(angle);
		}
	}
}

int ArrangeSpriteImpl::RotateSpriteState::
GetAnglePos(float angle)
{
	angle = -angle;
	angle += ee::PI / 8;
	angle = angle - ee::PI*2*std::floor(angle/(ee::PI*2));
	assert(angle >= 0);

	return std::floor(angle / (ee::PI / 4)) + 1;	
}

}