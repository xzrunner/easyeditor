#ifndef _PIMG_CONDENSE_H_
#define _PIMG_CONDENSE_H_

#include "Rect.h"

#include <stdint.h>

namespace pimg
{

class Condense
{
public:
	Condense(const uint8_t* pixels, int width, int height);

	Rect GetRegion() const;

	uint8_t* GetPixels(Rect& region) const;

private:
	bool IsTransparent(int x, int y) const;

private:
	const uint8_t* m_pixels;
	int m_width, m_height;

}; // Condense
 
}

#endif // _PIMG_CONDENSE_H_