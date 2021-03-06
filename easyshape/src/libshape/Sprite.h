#ifndef _EASYSHAPE_SPRITE_H_
#define _EASYSHAPE_SPRITE_H_

#include <ee/Sprite.h>

#include <sprite2/ShapeSprite.h>

namespace eshape
{

class Symbol;

class Sprite : public s2::ShapeSprite, public ee::Sprite
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

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(Sprite)

}; // Sprite

}

#endif // _EASYSHAPE_SPRITE_H_