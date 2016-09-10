#ifndef _GUM_TEXTBOX_LOADER_H_
#define _GUM_TEXTBOX_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class Textbox; }

namespace gum
{

class TextboxLoader : private cu::Uncopyable
{
public:
	TextboxLoader(s2::Textbox& tb);

	void LoadJson(const Json::Value& val);

private:
	s2::Textbox& m_tb;

}; // TextboxLoader

}

#endif // _GUM_TEXTBOX_LOADER_H_
