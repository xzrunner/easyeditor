#ifndef _EASYEDITOR_RENDER_UTILITY_H_
#define _EASYEDITOR_RENDER_UTILITY_H_

#include <vector>

namespace ee
{

class Sprite;

Sprite* draw_all_to_one_spr(const std::vector<Sprite*>& sprites, Sprite* except);
Sprite* draw_all_to_one_spr(const std::vector<Sprite*>& sprites);

void gl_debug();

}

#endif // _EASYEDITOR_RENDER_UTILITY_H_