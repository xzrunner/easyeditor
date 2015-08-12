#ifndef _DRAG2D_RIGHT_POPUP_MENU_H_
#define _DRAG2D_RIGHT_POPUP_MENU_H_

#include <wx/wx.h>

namespace d2d
{

class EditPanelImpl;
class MultiSpritesImpl;
class SpriteSelection;

class RightPopupMenu
{
public:
	RightPopupMenu(wxWindow* parent, EditPanelImpl* stage,
		MultiSpritesImpl* sprite_impl, SpriteSelection* selection);

	void SetRightPopupMenu(wxMenu& menu, int x, int y);
	void OnRightPopupMenu(int id);

private:
	void CreateCommonMenu(wxMenu& menu);
	void CreateSelectMenu(wxMenu& menu, int x, int y);
	void CreateDebugMenu(wxMenu& menu);

	void HandleCommonMenu(int id);
	void HandleSelectMenu(int id);
	void HandleDebugTagMenu(int id);

	void UpLayerMost();
	void DownLayerMost();

	void HoriMirror();
	void VertMirror();

private:
	enum
	{
		MENU_UP_MOST			= 10000,
		MENU_DOWN_MOST,
		MENU_HORI_MIRROR,
		MENU_VERT_MIRROR,

		MENU_MULTI_SELECTED		= 10100,
		MENU_MULTI_SELECTED_END	= 10199,

		MENU_INSERT_TO_DTEX		= 10200,
		MENU_REMOVE_FROM_DTEX
	};

private:
	wxWindow* m_parent;

	EditPanelImpl* m_stage;

	MultiSpritesImpl* m_sprites_impl;

	SpriteSelection* m_selection;

}; // RightPopupMenu

}

#endif // _DRAG2D_RIGHT_POPUP_MENU_H_