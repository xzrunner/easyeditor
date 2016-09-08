#ifndef _GLUE_TEXTBOX_SPR_LOADER_H_
#define _GLUE_TEXTBOX_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class TextboxSprite; }

namespace glue
{

class TextboxSprLoader : private cu::Uncopyable
{
public:
	TextboxSprLoader(s2::TextboxSprite* spr);
	~TextboxSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);

private:
	s2::TextboxSprite* m_spr;

}; // TextboxSprLoader

}

#endif // _GLUE_TEXTBOX_SPR_LOADER_H_