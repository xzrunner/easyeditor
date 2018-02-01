#ifndef _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_
#define _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_

#include "AtomicOP.h"
#include "Sprite.h"

#include <painting2/Color.h>

namespace ee
{

#define SET_SPRITE_COLOR_AOP_DEC(name)                                                       \
	class SetSprite##name##ColorAOP : public AtomicOP                                        \
	{                                                                                        \
	public:                                                                                  \
		SetSprite##name##ColorAOP(const SprPtr& spr, const pt2::Color& color);                \
		SetSprite##name##ColorAOP(const std::vector<SprPtr>& sprs, const pt2::Color& color);  \
	                                                                                         \
		virtual void Undo() override;                                                                 \
		virtual void Redo() override;                                                                 \
	                                                                                         \
		virtual Json::Value Store(const std::vector<SprPtr>& sprs) const override;                   \
	                                                                                         \
	private:                                                                                 \
		std::vector<SprPtr> m_sprs;                                                          \
		std::vector<pt2::Color> m_old_color;                                                  \
	                                                                                         \
		pt2::Color m_new_color;                                                               \
	};                                                                                       \

SET_SPRITE_COLOR_AOP_DEC(Mul)
SET_SPRITE_COLOR_AOP_DEC(Add)
SET_SPRITE_COLOR_AOP_DEC(TransR)
SET_SPRITE_COLOR_AOP_DEC(TransG)
SET_SPRITE_COLOR_AOP_DEC(TransB)

}

#endif // _EASYEDITOR_SET_SPRITE_COLOR_AOP_H_