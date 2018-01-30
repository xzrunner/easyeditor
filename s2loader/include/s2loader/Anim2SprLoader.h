#ifndef _S2LOADER_ANIM2_SPR_LOADER_H_
#define _S2LOADER_ANIM2_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class Anim2Sprite; }
namespace simp { class NodeAnim2Spr; }

namespace s2loader
{

class Anim2SprLoader : private cu::Uncopyable
{
public:
	Anim2SprLoader(s2::Anim2Sprite& spr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeAnim2Spr* node);

private:
	s2::Anim2Sprite& m_spr;

}; // Anim2SprLoader

}

#endif // _S2LOADER_ANIM2_SPR_LOADER_H_
