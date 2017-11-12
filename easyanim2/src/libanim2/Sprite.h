#ifndef _LIBANIM2_SPRITE_H_
#define _LIBANIM2_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/Anim2Sprite.h>

namespace libanim2
{

class Symbol;

class Sprite : public s2::Anim2Sprite, public ee::Sprite
{
public:
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual void Load(const Json::Value& val, const std::string& dir = "") override;
	virtual void Store(Json::Value& val, const std::string& dir = "") const override;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _LIBANIM2_SPRITE_H_