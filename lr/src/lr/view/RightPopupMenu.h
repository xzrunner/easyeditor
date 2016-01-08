#ifndef _LR_RIGHT_POPUP_MENU_H_
#define _LR_RIGHT_POPUP_MENU_H_

#include "dataset/CharacterFileName.h"

namespace lr
{

class StagePanel;

class RightPopupMenu
{
public:
	RightPopupMenu(d2d::LibraryPanel* library, StagePanel* stage);

	void SetRightPopupMenu(wxMenu& menu, int x, int y);
	void OnRightPopupMenu(int id);

private:
	void CreateShapeMenu(wxMenu& menu);
	void CreateAnimMenu(wxMenu& menu);
	void CreateLayerTagMenu(wxMenu& menu);
	void CreateLayerMoveMenu(wxMenu& menu);

	void HandleShapeMenu(int id);
	void HandleAnimMenu(int id);
	void HandleLayerTagMenu(int id);
	void HandleMoveToLayerMenu(int id);

	void FetchCandidateAnimFiles(const std::string& filepath);

private:
	static const int MENU_OPEN_WITH_SHAPE_ID	= 200;
	static const int MENU_ROTATE_LEFT_ID		= 201;
	static const int MENU_ROTATE_RIGHT_ID		= 202;

	static const int MENU_COLOR_START_ID		= 210;

	static const int MENU_LAYER_CLEAR_TAG_ID	= 250;
	static const int MENU_COVER_LAYER_TAG_ID	= 251;
	static const int MENU_TOP_LAYER_TAG_ID		= 252;

	static const int MENU_MOVE_TO_LAYER_BEGIN_ID= 260;
	static const int MENU_MOVE_TO_LAYER_END_ID	= 280;

private:
	d2d::LibraryPanel* m_library;

	StagePanel* m_stage;

	d2d::ISprite* m_sprite;

	std::vector<CharacterFileName> m_anim_files;

}; // RightPopupMenu

}

#endif // _LR_RIGHT_POPUP_MENU_H_