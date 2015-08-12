#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "RightPopupMenu.h"

#include "dataset/CharacterAllDirections.h"

#include <easycomplex.h>
#include <easyparticle3d.h>

namespace lr
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, 
									 d2d::PropertySettingPanel* property,
									 CharacterAllDirections* dirs)
	: d2d::ArrangeSpriteImpl(stage, stage->GetStageImpl(), stage, property)
	, m_popup(stage)
	, m_dirs(dirs)
{
}

void ArrangeSpriteImpl::OnPopMenuSelected(int type)
{
	d2d::ArrangeSpriteImpl::OnPopMenuSelected(type);
	m_popup.OnRightPopupMenu(type);
}

void ArrangeSpriteImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	d2d::ArrangeSpriteImpl::SetRightPopupMenu(menu, x, y);
	m_popup.SetRightPopupMenu(menu, x, y);
}

d2d::IArrangeSpriteState* ArrangeSpriteImpl::
CreateRotateState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const
{
	std::vector<d2d::ISprite*> sprites;
	selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
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
RotateSpriteState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos,
				  CharacterAllDirections* dirs)
	: d2d::RotateSpriteState(selection, first_pos)
	, m_dirs(dirs)
{
}

void ArrangeSpriteImpl::RotateSpriteState::
Rotate(const d2d::Vector& dst)
{
	std::vector<d2d::ISprite*> sprs;
	m_selection->Traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		d2d::ISprite* spr = sprs[i];
		std::string filepath = spr->GetSymbol().GetFilepath();
		if (!CharacterFileName::IsValidFilepath(filepath)) {
			d2d::Vector center = spr->GetPosition() + spr->GetOffset();
			float angle = d2d::Math::getAngleInDirection(center, m_last_pos, dst);
			spr->Rotate(angle);
		}
	}
}

int ArrangeSpriteImpl::RotateSpriteState::
GetAnglePos(float angle)
{
	angle = -angle;
	angle += d2d::PI / 8;
	angle = angle - d2d::PI*2*std::floor(angle/(d2d::PI*2));
	assert(angle >= 0);

	return std::floor(angle / (d2d::PI / 4)) + 1;	
}

}