#ifndef _EASYICON_SPRITE_H_
#define _EASYICON_SPRITE_H_

#include "Symbol.h"

namespace eicon
{

class Sprite : public d2d::Sprite
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
	virtual bool Update(int version) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::Symbol* symbol);
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;
	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);

	static d2d::Sprite* Create(d2d::Symbol* symbol) {
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