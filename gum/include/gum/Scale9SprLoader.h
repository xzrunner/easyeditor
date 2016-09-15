#ifndef _GUM_SCALE9_SPR_LOADER_H_
#define _GUM_SCALE9_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class Scale9Sprite; }
namespace simp { class NodeScale9Spr; }

namespace gum
{

class Scale9SprLoader : private cu::Uncopyable
{
public:
	Scale9SprLoader(s2::Scale9Sprite* spr);
	~Scale9SprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);
	void LoadBin(const simp::NodeScale9Spr* node);

private:
	s2::Scale9Sprite* m_spr;

}; // Scale9SprLoader

}

#endif // _GUM_SCALE9_SPR_LOADER_H_
