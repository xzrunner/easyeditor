#ifndef _LR_RIGHT_POPUP_MENU_H_
#define _LR_RIGHT_POPUP_MENU_H_

#include <drag2d.h>

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
	struct AnimFileName
	{
		AnimFileName(const std::string& filepath);

		bool IsValid() const { return !postfix.empty(); }

		bool SameExceptColor(const AnimFileName& name) const;

		std::string filepath;

		std::string character;
		std::string action;
		std::string direction;
		std::string color;
		std::string postfix;
	};

	static const int MENU_OPEN_WITH_SHAPE_ID = 200;
	static const int MENU_COLOR_START_ID = 201;

private:
	StagePanel* m_stage;

	d2d::ISprite* m_sprite;

	std::vector<AnimFileName> m_anim_files;

}; // RightPopupMenu

}

#endif // _LR_RIGHT_POPUP_MENU_H_