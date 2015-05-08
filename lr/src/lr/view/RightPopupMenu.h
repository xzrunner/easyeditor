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
	void FetchCandidateItems(const std::string& filepath);

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

	static const int MENU_START_ID = 200;

private:
	StagePanel* m_stage;

	d2d::ISprite* m_sprite;

	std::vector<AnimFileName> m_items;

}; // RightPopupMenu

}

#endif // _LR_RIGHT_POPUP_MENU_H_