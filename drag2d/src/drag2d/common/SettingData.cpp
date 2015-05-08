#include "SettingData.h"

namespace d2d
{

SettingData::SettingData()
{
	open_sprite_capture = true;
	auto_get_focus = true;

	open_image_edge_clip = true;
	visible_image_edge = false;

	visible_label_bg = true;
	visible_label_text = true;
	visible_node_name = false;
	visible_shape = true;

	linear_filter = true;

	bg_color.set(1, 1, 1);
}

void SettingData::LoadFromFile(const Json::Value& value)
{
	open_sprite_capture = value["sprite_capture"].asBool();
	if (!value["auto_get_focus"].isNull()) {
		auto_get_focus = value["auto_get_focus"].asBool();
	}

	open_image_edge_clip = value["image_edge_clip"].asBool();
	visible_image_edge = value["visible_image_edge"].asBool();

	visible_label_bg = value["visible_label_bg"].asBool();
	visible_label_text = value["visible_label_text"].asBool();
	visible_node_name = value["visible_node_name"].asBool();
	visible_shape = value["visible_shape"].asBool();

	linear_filter = value["linear_filter"].asBool();
}

}