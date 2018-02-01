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
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const override;

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual bool LoadResources() override;

	/**
	 *  @interface
	 *    s2::TextboxSymbol
	 */
	virtual void DrawText(cooking::DisplayList* dlist, const gtxt_label_style& style, const S2_MAT& mt, const pt2::Color& mul,
		const pt2::Color& add, const CU_STR& text, int time, bool richtext) const override;

private:
	void DrawBackground(const Sprite* spr, const S2_MAT& mt) const;

}; // Symbol

}

#endif // _EASYTEXT_SYMBOL_H_