#ifndef _EASYTEXT_SYMBOL_H_
#define _EASYTEXT_SYMBOL_H_

#include "typedef.h"

#include <ee/Symbol.h>

namespace etext
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const ee::RenderParams& trans, const ee::Sprite* spr = NULL, 
		const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void DrawBackground(const ee::Sprite* sprite, const sm::mat4& mt) const;
	void DrawText(const ee::Sprite* sprite, const ee::RenderParams& trans) const;

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

private:
	friend class Sprite;

}; // Symbol

}

#endif // _EASYTEXT_SYMBOL_H_