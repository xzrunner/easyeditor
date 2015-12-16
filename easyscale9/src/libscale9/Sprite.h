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
	virtual Sprite* Clone() const;

	//
	// d2d::ISprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(d2d::ISymbol* symbol);
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;
	virtual d2d::IPropertySetting* CreatePropertySetting(d2d::EditPanelImpl* stage);	

	void GetSize(float& w, float& h) const;
	void SetSize(float w, float h);

	void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans()) const;

	Scale9Type GetScale9Type() const { return m_data.GetType(); }

	const Scale9Data& GetScale9Data() const { return m_data; }

	static d2d::ISprite* Create(d2d::ISymbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}
	
private:
	Symbol* m_symbol;

	Scale9Data m_data;

}; // Sprite

}

#endif // _EASYSCALE9_SPRITE_H_