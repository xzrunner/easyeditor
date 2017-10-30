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
	static std::shared_ptr<Sprite> Group(const std::vector<ee::SprPtr>& sprs);
	static void BreakUp(const ee::SprPtr& group, std::vector<ee::SprPtr>& sprs);

	static void BuildComplex(const std::vector<ee::SprPtr>& sprs, const std::string& dir, wxWindow* wnd);
	static void BreakUpComplex(std::vector<ee::SprPtr>& sprs);

	static void BuildGroup(const std::vector<ee::SprPtr>& sprs);
	static void BreakUpGroup(std::vector<ee::SprPtr>& sprs);

	static void BreakUp(const ee::SprPtr& spr);

}; // GroupHelper

}

#endif // _EASYCOMPLEX_GROUP_HELPER_H_