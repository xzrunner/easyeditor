#ifndef _SPRITE2_IMAGE_SYMBOL_H_
#define _SPRITE2_IMAGE_SYMBOL_H_

#include "Symbol.h"

#include <stdint.h>

namespace s2
{

class ImageSymbol : public Symbol
{
public:
	ImageSymbol();

	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;

protected:
	uint32_t m_tex_id;
	uint16_t m_xmin, m_ymin, m_xmax, m_ymax;

}; // ImageSymbol

}

#endif // _SPRITE2_IMAGE_SYMBOL_H_