#ifndef _S2LOADER_TEXTBOX_SPR_LOADER_H_
#define _S2LOADER_TEXTBOX_SPR_LOADER_H_

#include <cu/uncopyable.h>

#include <json/json.h>

namespace s2 { class TextboxSprite; }
namespace simp { class NodeLabel; }

namespace s2loader
{

class TextboxSprLoader : private cu::Uncopyable
{
public:
	TextboxSprLoader(s2::TextboxSprite& spr);

	void LoadJson(const Json::Value& val);
	void LoadBin(const simp::NodeLabel* node);

private:
	s2::TextboxSprite& m_spr;

}; // TextboxSprLoader

}

#endif // _S2LOADER_TEXTBOX_SPR_LOADER_H_
