#ifndef _DRAG2D_RENDER_UTILITY_H_
#define _DRAG2D_RENDER_UTILITY_H_

#include <vector>

namespace d2d
{

class ISprite;
class MultiSpritesImpl;

ISprite* draw_all_to_one_spr(const MultiSpritesImpl* sprites_impl, ISprite* except);
ISprite* draw_all_to_one_spr(const std::vector<ISprite*>& sprites);

}

#endif // _DRAG2D_RENDER_UTILITY_H_