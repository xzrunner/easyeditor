#ifndef _EASYCOMPLEX_GROUP_HELPER_H_
#define _EASYCOMPLEX_GROUP_HELPER_H_

#include <ee/Sprite.h>

#include <vector>

class wxWindow;

namespace ee { class SpriteSelection; }

namespace ecomplex
{

class Sprite;

class GroupHelper
{
public:
	static Sprite* Group(const std::vector<ee::Sprite*>& sprs);
	static void BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprs);

	static void BuildComplex(ee::SpriteSelection* selection, const std::string& dir, wxWindow* wnd);
	static void BreakUpComplex(ee::SpriteSelection* selection);

	static void BuildGroup(ee::SpriteSelection* selection);
	static void BreakUpGroup(ee::SpriteSelection* selection);

	static void BreakUp(ee::Sprite* spr);

}; // GroupHelper

}

#endif // _EASYCOMPLEX_GROUP_HELPER_H_