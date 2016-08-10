#ifndef _EASYICON_SPRITE_H_
#define _EASYICON_SPRITE_H_

#include "Symbol.h"

#include <ee/Sprite.h>

namespace eicon
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
	virtual bool Update(const s2::RenderParams& params, float dt) { return true; }
	virtual const Symbol& GetSymbol() const;
	virtual void SetSymbol(ee::Symbol* symbol);

	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	static ee::Sprite* Create(ee::Symbol* symbol) {
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