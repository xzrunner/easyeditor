#ifndef _EASYAUDIO_SPRITE_H_
#define _EASYAUDIO_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/AudioSprite.h>

namespace eaudio
{

class Symbol;

class Sprite : public s2::AudioSprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual Sprite* Clone() const { return new Sprite(*this); }
	virtual bool Update(const s2::UpdateParams& up);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYAUDIO_SPRITE_H_