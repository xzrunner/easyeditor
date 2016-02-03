#ifndef _EASYTEXT_SPRITE_H_
#define _EASYTEXT_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>

namespace etext
{

class Sprite : public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// Cloneable interface
	//
	virtual Sprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(int version);
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	void GetSize(int& width, int& height) const;
	void SetSize(int width, int height);

	int GetFont() const { return m_font; }
	void SetFont(int font) { m_font = font; }

	int GetFontSize() const { return m_font_size; }
	void SetFontSize(int size) { m_font_size = size; }

	const ee::Colorf& GetFontColor() const { return m_font_color; }
	ee::Colorf& GetFontColor() { return m_font_color; }
	void SetFontColor(const ee::Colorf& col) { m_font_color = col; }

	bool GetEdge() const { return m_edge; }
	void SetEdge(bool edge) { m_edge = edge; }

	float GetEdgeSize() const { return m_edge_size; }
	void SetEdgeSize(float size) { m_edge_size = size; }

	const ee::Colorf& GetEdgeColor() const { return m_edge_color; }
	void SetEdgeColor(const ee::Colorf& col) { m_edge_color = col; }

	void GetAlign(int& halign, int& valign) const;
	void SetAlign(int halign, int valign);

	void GetSpace(float& hori, float& vert) const;
	void SetSpace(float hori, float vert);

	const std::string& GetText() const { return m_text; }
	void SetText(const std::string& text) { m_text = text; }

	const std::string& GetTID() const { return m_tid; }
	void SetTID(const std::string& tid) { m_tid = tid; }	

	int GetTime() const { return m_time; }
	void UpdateTime() const { ++m_time; }

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

private:
	int m_width;
	int m_height;

	int m_font;
	int m_font_size;
	ee::Colorf m_font_color;

	bool m_edge;
	float m_edge_size;
	ee::Colorf m_edge_color;

	HoriAlignType m_align_hori;
	VertAlignType m_align_vert;

	float m_space_hori;
	float m_space_vert;

private:
	std::string m_text;
	std::string m_tid;

private:
	mutable int m_time;	 // for gtxt dynamic draw

private:
	Symbol* m_symbol;

}; // Sprite

}

#endif // _EASYTEXT_SPRITE_H_