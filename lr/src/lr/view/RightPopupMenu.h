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
	void FetchCandidateAnimFiles(const std::string& filepath);

private:
	static const int MENU_OPEN_WITH_SHAPE_ID = 200;
	static const int MENU_COLOR_START_ID = 201;

private:
	StagePanel* m_stage;

	d2d::ISprite* m_sprite;

	std::vector<CharacterFileName> m_anim_files;

}; // RightPopupMenu

}

#endif // _LR_RIGHT_POPUP_MENU_H_