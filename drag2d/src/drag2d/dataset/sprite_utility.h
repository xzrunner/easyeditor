#ifndef _DRAG2D_SPRITE_UTILITY_H_
#define _DRAG2D_SPRITE_UTILITY_H_

namespace d2d
{

class ISprite;

float get_spr_left(const ISprite* spr);
float get_spr_right(const ISprite* spr);
float get_spr_down(const ISprite* spr);
float get_spr_up(const ISprite* spr);

float get_spr_center_x(const ISprite* spr);
float get_spr_center_y(const ISprite* spr);

}

#endif // _DRAG2D_SPRITE_UTILITY_H_