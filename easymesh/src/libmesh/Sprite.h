#ifndef _EASYMESH_SPRITE_H_
#define _EASYMESH_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/MeshSprite.h>

namespace emesh
{

class Symbol;

class Sprite : public s2::MeshSprite, public ee::Sprite
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

	static ee::Sprite* Create(ee::Symbol* sym);

}; // Sprite

}

#endif // _EASYMESH_SPRITE_H_