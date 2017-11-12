#ifndef _EASYPARTICLE2D_SPRITE_H_
#define _EASYPARTICLE2D_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/Particle2dSprite.h>

namespace eparticle2d
{

class Symbol;

class Sprite : public s2::Particle2dSprite, public ee::Sprite
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
	virtual void Load(const Json::Value& val, const std::string& dir = "") override;
	virtual void Store(Json::Value& val, const std::string& dir = "") const override;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYPARTICLE2D_SPRITE_H_