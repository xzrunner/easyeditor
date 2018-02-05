#ifndef _S2LOADER_ICON_SPR_LOADER_H_
#define _S2LOADER_ICON_SPR_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

#include <json/json.h>

namespace s2 { class IconSprite; }
namespace simp { class NodeIconSpr; }
namespace s2s { class IconSpr; }

namespace s2loader
{

class IconSprLoader : private cu::Uncopyable
{
public:
	IconSprLoader(s2::IconSprite& spr);

	void LoadJson(const Json::Value& val, const CU_STR& dir);
	void LoadBin(const simp::NodeIconSpr* node);
	void LoadSns(const s2s::IconSpr* spr);

private:
	s2::IconSprite& m_spr;

}; // IconSprLoader

}

#endif // _S2LOADER_ICON_SPR_LOADER_H_
