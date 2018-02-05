#ifndef _S2LOADER_ANIM_SPR_LOADER_H_
#define _S2LOADER_ANIM_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class AnimSprite; }
namespace simp { class NodeAnimationSpr; }
namespace s2s { class NodeSpr; }

namespace s2loader
{

class AnimSprLoader : private cu::Uncopyable
{
public:
	AnimSprLoader(s2::AnimSprite& spr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeAnimationSpr* node);
	void LoadSns(const s2s::NodeSpr* spr);

private:
	s2::AnimSprite& m_spr;

}; // AnimSprLoader

}

#endif // _S2LOADER_ANIM_SPR_LOADER_H_
