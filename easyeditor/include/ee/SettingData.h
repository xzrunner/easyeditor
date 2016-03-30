#ifndef _EASYEDITOR_SETTING_DATA_H_
#define _EASYEDITOR_SETTING_DATA_H_

#include "Color.h"

#include <json/json.h>

namespace ee
{

enum COL_SET_DLG_TYPE {
	CSDT_DEFAULT = 0,
	CSDT_RGB,
	CSDT_HSL
};

class SettingData
{
public:
	static int ctl_pos_sz;
	static bool draw_tris_edge, draw_poly_bound;

	enum DrawType
	{
		e_DrawBoth = 0,
		e_DrawPhysics,
		e_DrawImage
	};
	static DrawType draw_type;

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

	// widgets
	bool img_list_compact;

	// image filter
	bool linear_filter;

	// image loading
	bool pre_multi_alpha;
	bool load_image;

	// widget
	COL_SET_DLG_TYPE color_setting_dlg_type;

	// draw
	bool draw_anchor;

public:
	SettingData();

	void LoadFromFile(const Json::Value& value);

}; // SettingData

}

#endif // _EASYEDITOR_SETTING_DATA_H_