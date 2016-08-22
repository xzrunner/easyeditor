#ifndef _EASYSCALE9_SPRITE_H_
#define _EASYSCALE9_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/Scale9Sprite.h>

namespace escale9
{

class Symbol;

class Sprite : public s2::Scale9Sprite, public ee::Sprite
{
public:
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

	static ee::Sprite* Create(ee::Symbol* sym);
	
}; // Sprite

}

#endif // _EASYSCALE9_SPRITE_H_