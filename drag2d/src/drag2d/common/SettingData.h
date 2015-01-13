#ifndef _DRAG2D_SETTING_DATA_H_
#define _DRAG2D_SETTING_DATA_H_

#include <json/json.h>

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

	// image filter
	bool linear_filter;

	SettingData();

	void LoadFromFile(const Json::Value& value);

}; // SettingData

}

#endif // _DRAG2D_SETTING_DATA_H_