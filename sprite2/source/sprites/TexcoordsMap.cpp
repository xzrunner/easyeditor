#include "TexcoordsMap.h"

namespace s2
{

void TexcoordsMap::Trans(float src[8], sm::vec2 dst[4])
{
	float minx = src[0],
		  miny = src[1];
	float w = src[4] - minx,
		  h = src[5] - miny;
	if (h < 0) {
		for (int i = 0; i < 4; ++i) {
			float y = miny + h * dst[i].x,
				  x = minx + w * dst[i].y;
			dst[i].Set(x, y);
		}		
	} else {
		for (int i = 0; i < 4; ++i) {
			float x = minx + w * dst[i].x,
				  y = miny + h * dst[i].y;
			dst[i].Set(x, y);
		}
	}
}

}