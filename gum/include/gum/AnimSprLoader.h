#ifndef _GUM_ANIM_SPR_LOADER_H_
#define _GUM_ANIM_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class AnimSprite; }

namespace gum
{

class AnimSprLoader : private cu::Uncopyable
{
public:
	AnimSprLoader(s2::AnimSprite* spr);
	~AnimSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);

private:
	s2::AnimSprite* m_spr;

}; // AnimSprLoader

}

#endif // _GUM_ANIM_SPR_LOADER_H_
