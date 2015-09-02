#ifndef _DRAG2D_SETTING_DATA_H_
#define _DRAG2D_SETTING_DATA_H_

#include <json/json.h>

#include "common/Color.h"

namespace d2d
{

struct SettingData
{
	// edit
	bool open_sprite_capture;
	bool auto_get_focus;

	// picture
	bool open_image_edge_clip;
	bool visible_image_edge;

	// view
	bool visible_label_bg;
	bool visible_label_text;
	bool visible_node_name;
	float node_name_scale;
	bool visible_shape;
	bool visible_tex_edge;
	bool particle3d_loop;
	Colorf bg_color;
	bool visible_spr_update;

	// image filter
	bool linear_filter;

	// image loading
	bool pre_multi_alpha;
	bool load_image;

	SettingData();

	void LoadFromFile(const Json::Value& value);

}; // SettingData

}

#endif // _DRAG2D_SETTING_DATA_H_