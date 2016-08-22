#ifndef _EASYICON_SPRITE_H_
#define _EASYICON_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/IconSprite.h>

namespace eicon
{

class Symbol;

class Sprite : public s2::IconSprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	void SetProcess(float process);
	float GetProcess() const { return m_process; }

	static ee::Sprite* Create(ee::Symbol* sym);

private:
	float m_process;

}; // Sprite

}

#endif // _EASYICON_SPRITE_H_