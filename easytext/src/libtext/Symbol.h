#ifndef _EASYTEXT_SYMBOL_H_
#define _EASYTEXT_SYMBOL_H_

#include "typedef.h"

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
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void DrawBackground(const Sprite* sprite, const sm::mat4& mt) const;
	void DrawText(const Sprite* sprite, const s2::RenderParams& params) const;

private:
	int m_width;
	int m_height;

	int m_font;
	int m_font_size;
	std::string m_font_color;

	bool m_edge;
	float m_edge_size;
	std::string m_edge_color;

	HoriAlignType m_align_hori;
	VertAlignType m_align_vert;

	float m_space_hori;
	float m_space_vert;

	bool m_overflow;

	bool m_richtext;

private:
	friend class Sprite;

}; // Symbol

}

#endif // _EASYTEXT_SYMBOL_H_