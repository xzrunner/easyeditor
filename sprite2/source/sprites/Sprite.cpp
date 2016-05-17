#include "Sprite.h"

namespace s2
{

Sprite::Sprite(void* ud)
	: m_position(0, 0)
	, m_angle(0)
	, m_scale(1, 1)
	, m_shear(0, 0)
	, m_ud(ud)
{
}

// void Sprite::MultiplyRenderParams(const RenderParams& src, RenderParams& dst) const
// {
// 	
// }

}