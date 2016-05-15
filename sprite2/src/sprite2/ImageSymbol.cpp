#include "ImageSymbol.h"

namespace s2
{

ImageSymbol::ImageSymbol(void* ud)
	: Symbol(ud)
	, m_tex_id(0)
	, m_xmin(0)
	, m_ymin(0)
	, m_xmax(0)
	, m_ymax(0)
{
}

void ImageSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	
}

}