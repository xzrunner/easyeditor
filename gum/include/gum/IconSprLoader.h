#ifndef _GUM_ICON_SPR_LOADER_H_
#define _GUM_ICON_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class IconSprite; }
namespace simp { class NodeIconSpr; }

namespace gum
{

class IconSprLoader : private cu::Uncopyable
{
public:
	IconSprLoader(s2::IconSprite* spr);
	~IconSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);
	void LoadBin(const simp::NodeIconSpr* node);

private:
	s2::IconSprite* m_spr;

}; // IconSprLoader

}

#endif // _GUM_ICON_SPR_LOADER_H_
