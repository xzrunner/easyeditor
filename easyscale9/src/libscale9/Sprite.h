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
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);	

	void GetSize(float& w, float& h) const;
	void SetSize(float w, float h);

	void Draw(const s2::RenderParams& params) const;

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