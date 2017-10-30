#ifndef _EASYEDITOR_RENDER_UTILITY_H_
#define _EASYEDITOR_RENDER_UTILITY_H_

#include "Sprite.h"

#include <vector>

namespace ee
{

SprPtr draw_all_to_one_spr(const std::vector<SprPtr>& sprs, const SprPtr& except);
SprPtr draw_all_to_one_spr(const std::vector<SprPtr>& sprs);

void gl_debug();

}

#endif // _EASYEDITOR_RENDER_UTILITY_H_