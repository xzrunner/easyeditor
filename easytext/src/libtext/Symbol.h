#ifndef _EASYTEXT_SYMBOL_H_
#define _EASYTEXT_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/TextboxSymbol.h>

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
	virtual void LoadResources();

private:
	void DrawBackground(const Sprite* spr, const sm::mat4& mt) const;
	void DrawText(const Sprite* spr, const s2::RenderParams& params) const;

}; // Symbol

}

#endif // _EASYTEXT_SYMBOL_H_