#ifndef _EASYCOMPLEX_GROUP_HELPER_H_
#define _EASYCOMPLEX_GROUP_HELPER_H_

#include <ee/Sprite.h>

#include <vector>

class wxWindow;

namespace ecomplex
{

class Sprite;

class GroupHelper
{
public:
	static Sprite* Group(const std::vector<ee::Sprite*>& sprs);
	static void BreakUp(ee::Sprite* group, std::vector<ee::Sprite*>& sprs);

	static void BuildComplex(const std::vector<ee::Sprite*>& sprs, const std::string& dir, wxWindow* wnd);
	static void BreakUpComplex(std::vector<ee::Sprite*>& sprs);

	static void BuildGroup(const std::vector<ee::Sprite*>& sprs);
	static void BreakUpGroup(std::vector<ee::Sprite*>& sprs);

	static void BreakUp(ee::Sprite* spr);

}; // GroupHelper

}

#endif // _EASYCOMPLEX_GROUP_HELPER_H_