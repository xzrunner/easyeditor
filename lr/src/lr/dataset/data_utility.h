#ifndef _LR_DATA_UTILITY_H_
#define _LR_DATA_UTILITY_H_



namespace etexture { class Sprite; }

namespace lr
{

void create_shapes_from_etxture(etexture::Sprite* spr, std::vector<ee::Shape*>& shapes);

}

#endif // _LR_DATA_UTILITY_H_