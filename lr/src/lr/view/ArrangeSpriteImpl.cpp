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
#include <ee/Math2D.h>

#include <easycomplex.h>
#include <easyparticle3d.h>

#include <SM_Calc.h>

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

bool ArrangeSpriteImpl::OnKeyDown(int key_code)
{
	bool ret = ee::ArrangeSpriteImpl::OnKeyDown(key_code);

	if (m_stage->GetKeyState(WXK_SHIFT))
	{
		if (key_code == 'O') {
			ee::CameraMgr::Instance()->SetCamera(ee::CameraMgr::ORTHO);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			ret = true;
		} else if (key_code == 'P') {
			ee::CameraMgr::Instance()->SetCamera(ee::CameraMgr::PSEUDO3D);
			ee::SetCanvasDirtySJ::Instance()->SetDirty();
			ret = true;
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
				cam->Rotate(1);
				ret = true;
				break;
			case WXK_PAGEUP:
				cam->Rotate(-1);
				ret = true;
				break;
			}
		}
	}
	return ret;
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
CreateRotateState(ee::SpriteSelection* selection, const sm::vec2& first_pos) const
{
	std::vector<ee::Sprite*> sprs;
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
	if (sprs.size() == 1 && dynamic_cast<eparticle3d::Sprite*>(sprs[0])) {
		return new ecomplex::SphereRotateState(m_stage, first_pos, static_cast<eparticle3d::Sprite*>(sprs[0])->GetDir());
	} else {
		return new RotateSpriteState(selection, first_pos, m_dirs);
	}
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteImpl::RotateSpriteState
//////////////////////////////////////////////////////////////////////////

ArrangeSpriteImpl::RotateSpriteState::
RotateSpriteState(ee::SpriteSelection* selection, const sm::vec2& first_pos,
				  CharacterAllDirections* dirs)
	: ee::RotateSpriteState(selection, first_pos)
	, m_dirs(dirs)
{
}

void ArrangeSpriteImpl::RotateSpriteState::
Rotate(const sm::vec2& dst)
{
	std::vector<ee::Sprite*> sprs;
	m_selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];
		std::string filepath = dynamic_cast<const ee::Symbol*>(spr->GetSymbol())->GetFilepath();
		if (!CharacterFileName::IsValidFilepath(filepath)) {
			sm::vec2 center = spr->GetPosition() + spr->GetOffset();
			float angle = sm::get_angle_in_direction(center, m_last_pos, dst);
			spr->Rotate(angle);
		}
	}
}

int ArrangeSpriteImpl::RotateSpriteState::
GetAnglePos(float angle)
{
	angle = -angle;
	angle += SM_PI / 8;
	angle = angle - SM_PI*2*std::floor(angle/(SM_PI*2));
	assert(angle >= 0);

	return std::floor(angle / (SM_PI / 4)) + 1;	
}

}