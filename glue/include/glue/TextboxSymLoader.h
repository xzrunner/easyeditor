#ifndef _GLUE_TEXTBOX_SYM_LOADER_H_
#define _GLUE_TEXTBOX_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

#include <string>

namespace s2 { class TextboxSymbol; }

namespace glue
{

class TextboxSymLoader : private cu::Uncopyable
{
public:
	TextboxSymLoader(s2::TextboxSymbol* sym);
	~TextboxSymLoader();

	void LoadJson(const std::string& filepath);

private:
	s2::TextboxSymbol* m_sym;

}; // TextboxSymLoader

}

#endif // _GLUE_TEXTBOX_SYM_LOADER_H_