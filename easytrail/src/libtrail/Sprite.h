#ifndef _EASYTRAIL_SPRITE_H_
#define _EASYTRAIL_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/TrailSprite.h>

namespace etrail
{

class Symbol;

class Sprite : public s2::TrailSprite, public ee::Sprite
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "");
	virtual void Store(Json::Value& val, const std::string& dir = "") const;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	SPRITE_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYTRAIL_SPRITE_H_