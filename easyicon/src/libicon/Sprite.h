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
	Sprite() {}
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const CU_STR& dir = "");
	virtual void Store(Json::Value& val, const CU_STR& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	SPRITE_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYICON_SPRITE_H_