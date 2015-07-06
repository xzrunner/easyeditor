#include "ArrangeSpriteImpl.h"
#include "StagePanel.h"
#include "RightPopupMenu.h"

#include "dataset/CharacterAllDirections.h"

#include <easycomplex.h>

namespace lr
{

ArrangeSpriteImpl::ArrangeSpriteImpl(StagePanel* stage, 
									 d2d::PropertySettingPanel* property,
									 CharacterAllDirections* dirs)
	: d2d::ArrangeSpriteImpl(stage, stage, property)
	, m_popup(stage)
	, m_dirs(dirs)
{
}

void ArrangeSpriteImpl::OnPopMenuSelected(int type)
{
	d2d::ArrangeSpriteImpl::OnPopMenuSelected(type);
	m_popup.OnRightPopupMenu(type);
}

void ArrangeSpriteImpl::SetRightPopupMenu(wxMenu& menu, d2d::ISprite* spr)
{
	d2d::ArrangeSpriteImpl::SetRightPopupMenu(menu, spr);
	m_popup.SetRightPopupMenu(menu, spr);
}

d2d::IArrangeSpriteState* ArrangeSpriteImpl::
CreateRotateState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const
{
	return new RotateSpriteState(selection, first_pos, m_dirs);
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
		std::string filepath = spr->getSymbol().GetFilepath();
		if (!CharacterFileName::IsValidFilepath(filepath)) {
			d2d::Vector center = spr->getPosition() + spr->getOffset();
			float angle = d2d::Math::getAngleInDirection(center, m_last_pos, dst);
			spr->rotate(angle);
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