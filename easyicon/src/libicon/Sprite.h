#ifndef _EASYICON_SPRITE_H_
#define _EASYICON_SPRITE_H_

#include "Symbol.h"

namespace eicon
{

class Sprite : public d2d::ISprite
{
public:
	Sprite();
	Sprite(const Sprite& sprite);
	Sprite(Symbol* symbol);
	virtual ~Sprite();

	//
	// ICloneable interface
	//
	virtual Sprite* clone() const;

	//
	// ISprite interface
	//
	virtual const Symbol& getSymbol() const;
	virtual void setSymbol(d2d::ISymbol* symbol);
	virtual void load(const Json::Value& val);
	virtual void store(Json::Value& val) const;

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}

	void SetProcess(float process);
	float GetProcess() const { return m_process; }

private:
	Symbol* m_symbol;

	float m_process;

}; // Sprite

}

#endif // _EASYICON_SPRITE_H_