#ifndef _LR_ARRANGE_SPRITE_IMPL_H_
#define _LR_ARRANGE_SPRITE_IMPL_H_

#include <drag2d.h>

#include "RightPopupMenu.h"

namespace lr
{

class StagePanel;
class RightPopupMenu;
class CharacterAllDirections;

class ArrangeSpriteImpl : public d2d::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(StagePanel* stage, d2d::PropertySettingPanel* property,
		CharacterAllDirections* dirs, d2d::ViewPanelMgr* view_panel_mgr);

	virtual void OnPopMenuSelected(int type);

protected:
	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

	virtual d2d::IArrangeSpriteState* CreateRotateState(d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos) const;

private:
	class RotateSpriteState : public d2d::RotateSpriteState
	{
	public:
		RotateSpriteState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos,
			CharacterAllDirections* dirs);

	protected:
		virtual void Rotate(const d2d::Vector& dst);

	private:
		static int GetAnglePos(float angle);

	private:
		CharacterAllDirections* m_dirs;

	}; // RotateSpriteState

private:
	RightPopupMenu m_popup;

	CharacterAllDirections* m_dirs;

}; // ArrangeSpriteImpl

}

#endif // _LR_ARRANGE_SPRITE_IMPL_H_