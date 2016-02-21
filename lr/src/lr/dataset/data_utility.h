#ifndef _LR_DATA_UTILITY_H_
#define _LR_DATA_UTILITY_H_

#include <vector>

namespace etexture { class Sprite; }
namespace ee { class Shape; }

namespace lr
{

void create_shapes_from_etxture(etexture::Sprite* spr, std::vector<ee::Shape*>& shapes);

}

#endif // _LR_DATA_UTILITY_H_