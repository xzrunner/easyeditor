#ifndef _LR_RIGHT_POPUP_MENU_H_
#define _LR_RIGHT_POPUP_MENU_H_

#include "dataset/CharacterFileName.h"

namespace lr
{

class StagePanel;

class RightPopupMenu
{
public:
	RightPopupMenu(StagePanel* stage);

	void SetRightPopupMenu(wxMenu& menu, d2d::ISprite* spr);
	void OnRightPopupMenu(int id);

private:
	void CreateShapeMenu(wxMenu& menu);
	void CreateAnimMenu(wxMenu& menu);
	void CreateLayerTagMenu(wxMenu& menu);

	void HandleShapeMenu(int id);
	void HandleAnimMenu(int id);
	void HandleLayerTagMenu(int id);

	void FetchCandidateAnimFiles(const std::string& filepath);

private:
	static const int MENU_OPEN_WITH_SHAPE_ID	= 200;
	static const int MENU_ROTATE_LEFT_ID		= 201;
	static const int MENU_ROTATE_RIGHT_ID		= 202;

	static const int MENU_COLOR_START_ID		= 210;

	static const int MENU_COVER_LAYER_TAG_ID	= 250;
	static const int MENU_TOP_LAYER_TAG_ID		= 251;

private:
	StagePanel* m_stage;

	d2d::ISprite* m_sprite;

	std::vector<CharacterFileName> m_anim_files;

}; // RightPopupMenu

}

#endif // _LR_RIGHT_POPUP_MENU_H_