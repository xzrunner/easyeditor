#ifndef _DRAG2D_ARRANGE_SPRITE_CONFIG_H_
#define _DRAG2D_ARRANGE_SPRITE_CONFIG_H_

namespace d2d
{

struct ArrangeSpriteConfig
{
	bool is_auto_align_open;

	bool is_deform_open;
	bool is_offset_open;
	bool is_rotate_open;

	ArrangeSpriteConfig() {
		is_auto_align_open = true;
		is_deform_open = true;
		is_offset_open = true;
		is_rotate_open = true;
	}

	ArrangeSpriteConfig(bool auto_align, bool deform, bool offset, bool rotate) {
		is_auto_align_open = auto_align;
		is_deform_open = deform;
		is_offset_open = offset;
		is_rotate_open = rotate;
	}

}; // ArrangeSpriteConfig

}

#endif _DRAG2D_ARRANGE_SPRITE_CONFIG_H_