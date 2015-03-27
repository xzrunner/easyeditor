#ifndef _EASYSCALE9_SPRITE_H_
#define _EASYSCALE9_SPRITE_H_

#include "Symbol.h"

namespace escale9
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// IObject interface
	//
	virtual Sprite* clone() const;

	//
	// d2d::ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);

	virtual void load(const Json::Value& val);
	virtual void store(Json::Value& val) const;

	void GetSize(float& w, float& h) const;
	void SetSize(float w, float h);

	void Draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0)) const;

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

	static void ResizeScale9(Symbol::Type type, d2d::ISprite* const sprites[3][3],
		float width, float height);
	static void ResizeSprite(d2d::ISprite* sprite, const d2d::Vector& center, 
		float width, float height);

private:
	void LoadFromSymbol(Symbol* symbol);
	
protected:
	Symbol* m_symbol;

private:
	float m_width, m_height;

	// 2
	// 1
	// 0 1 2
	d2d::ISprite* m_sprites[3][3];

}; // Sprite

}

#endif // _EASYSCALE9_SPRITE_H_