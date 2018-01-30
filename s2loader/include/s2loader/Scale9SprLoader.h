#ifndef _S2LOADER_SCALE9_SPR_LOADER_H_
#define _S2LOADER_SCALE9_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class Scale9Sprite; }
namespace simp { class NodeScale9Spr; }

namespace s2loader
{

class Scale9SprLoader : private cu::Uncopyable
{
public:
	Scale9SprLoader(s2::Scale9Sprite& spr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeScale9Spr* node);

private:
	s2::Scale9Sprite& m_spr;

}; // Scale9SprLoader

}

#endif // _S2LOADER_SCALE9_SPR_LOADER_H_
