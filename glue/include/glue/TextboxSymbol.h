#ifndef _GLUE_TEXTBOX_SYMBOL_H_
#define _GLUE_TEXTBOX_SYMBOL_H_

#include <sprite2/TextboxSymbol.h>

#include <string>

namespace glue
{

class TextboxSymbol : public s2::TextboxSymbol
{
public:

	void LoadJson(const std::string& filepath);

protected:
	/**
	 *  @interface
	 *    s2::TextboxSymbol
	 */
	virtual void DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
		const s2::Color& add, const std::string& text, int time, bool richtext) const;

}; // TextboxSymbol

}

#endif // _GLUE_TEXTBOX_SYMBOL_H_