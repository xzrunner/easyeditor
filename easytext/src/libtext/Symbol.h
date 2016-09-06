#ifndef _EASYTEXT_SYMBOL_H_
#define _EASYTEXT_SYMBOL_H_

#include <ee/Symbol.h>

#include <glue/TextboxSymbol.h>

namespace etext
{

class Sprite;

class Symbol : public ee::Symbol, public glue::TextboxSymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void LoadResources();

	/**
	 *  @interface
	 *    s2::TextboxSymbol
	 */
	virtual void DrawText(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
		const s2::Color& add, const std::string& text, int time, bool richtext) const;

private:
	void DrawBackground(const Sprite* spr, const sm::mat4& mt) const;

}; // Symbol

}

#endif // _EASYTEXT_SYMBOL_H_