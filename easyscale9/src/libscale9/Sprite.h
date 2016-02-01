#ifndef _EASYSCALE9_SPRITE_H_
#define _EASYSCALE9_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>

namespace escale9
{

class Sprite : public ee::Sprite
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
	// ee::Sprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);	

	void GetSize(float& w, float& h) const;
	void SetSize(float w, float h);

	void Draw(const ee::Matrix& mt, const ee::ColorTrans& color = ee::ColorTrans()) const;

	Scale9Type GetScale9Type() const { return m_data.GetType(); }

	const Scale9Data& GetScale9Data() const { return m_data; }

	static ee::Sprite* Create(ee::Symbol* symbol) {
		return new Sprite(static_cast<Symbol*>(symbol));
	}
	
private:
	Symbol* m_symbol;

	Scale9Data m_data;

}; // Sprite

}

#endif // _EASYSCALE9_SPRITE_H_