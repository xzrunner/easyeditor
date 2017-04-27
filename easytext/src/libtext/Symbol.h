#ifndef _EASYTEXT_SYMBOL_H_
#define _EASYTEXT_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/TextboxSymbol.h>

#include S2_MAT_HEADER

namespace etext
{

class Sprite;

class Symbol : public ee::Symbol, public s2::TextboxSymbol
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
	virtual bool LoadResources();

	/**
	 *  @interface
	 *    s2::TextboxSymbol
	 */
	virtual void DrawText(const gtxt_label_style& style, const S2_MAT& mt, const s2::Color& mul, 
		const s2::Color& add, const std::string& text, int time, bool richtext) const;

private:
	void DrawBackground(const Sprite* spr, const S2_MAT& mt) const;

}; // Symbol

}

#endif // _EASYTEXT_SYMBOL_H_