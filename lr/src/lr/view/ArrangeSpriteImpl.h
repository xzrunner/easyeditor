#ifndef _LR_ARRANGE_SPRITE_IMPL_H_
#define _LR_ARRANGE_SPRITE_IMPL_H_



#include "RightPopupMenu.h"

namespace lr
{

class StagePanel;
class RightPopupMenu;
class CharacterAllDirections;

class ArrangeSpriteImpl : public ee::ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(ee::LibraryPanel* library, ee::PropertySettingPanel* property,
		StagePanel* stage, CharacterAllDirections* dirs);

	virtual void OnPopMenuSelected(int type);

protected:
	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

	virtual ee::ArrangeSpriteState* CreateRotateState(ee::SpriteSelection* selection, 
		const ee::Vector& first_pos) const;

private:
	class RotateSpriteState : public ee::RotateSpriteState
	{
	public:
		RotateSpriteState(ee::SpriteSelection* selection, const ee::Vector& first_pos,
			CharacterAllDirections* dirs);

	protected:
		virtual void Rotate(const ee::Vector& dst);

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