#ifndef _EASYEDITOR_SPRITE_UTILITY_H_
#define _EASYEDITOR_SPRITE_UTILITY_H_

namespace ee
{

class Sprite;

float get_spr_left(const Sprite* spr);
float get_spr_right(const Sprite* spr);
float get_spr_down(const Sprite* spr);
float get_spr_up(const Sprite* spr);

float get_spr_center_x(const Sprite* spr);
float get_spr_center_y(const Sprite* spr);

}

#endif // _EASYEDITOR_SPRITE_UTILITY_H_