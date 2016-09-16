#ifndef _GUM_TEXTBOX_SPR_LOADER_H_
#define _GUM_TEXTBOX_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class TextboxSprite; }
namespace simp { class NodeLabel; }

namespace gum
{

class TextboxSprLoader : private cu::Uncopyable
{
public:
	TextboxSprLoader(s2::TextboxSprite* spr);
	~TextboxSprLoader();

	void LoadJson(const Json::Value& val);
	void LoadBin(const simp::NodeLabel* node);

private:
	s2::TextboxSprite* m_spr;

}; // TextboxSprLoader

}

#endif // _GUM_TEXTBOX_SPR_LOADER_H_