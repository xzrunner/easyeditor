#ifndef _LR_DATA_UTILITY_H_
#define _LR_DATA_UTILITY_H_

#include <drag2d.h>

namespace etexture { class Sprite; }

namespace lr
{

void create_shapes_from_etxture(etexture::Sprite* spr, std::vector<d2d::IShape*>& shapes);

}

#endif // _LR_DATA_UTILITY_H_