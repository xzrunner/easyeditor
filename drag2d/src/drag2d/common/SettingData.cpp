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
	particle3d_loop = true;
	bg_color.set(0.5f, 0.5f, 0.5f);

	linear_filter = true;
}

void SettingData::LoadFromFile(const Json::Value& value)
{
	if (!value["sprite_capture"].isNull()) {
		open_sprite_capture = value["sprite_capture"].asBool();
	}
	if (!value["auto_get_focus"].isNull()) {
		auto_get_focus = value["auto_get_focus"].asBool();
	}

	if (!value["image_edge_clip"].isNull()) {
		open_image_edge_clip = value["image_edge_clip"].asBool();
	}
	if (!value["visible_image_edge"].isNull()) {
		visible_image_edge = value["visible_image_edge"].asBool();
	}

	if (!value["visible_label_bg"].isNull()) {
		visible_label_bg = value["visible_label_bg"].asBool();
	}
	if (!value["visible_label_text"].isNull()) {
		visible_label_text = value["visible_label_text"].asBool();
	}
	if (!value["visible_node_name"].isNull()) {
		visible_node_name = value["visible_node_name"].asBool();
	}
	if (!value["visible_shape"].isNull()) {
		visible_shape = value["visible_shape"].asBool();
	}
	if (!value["particle3d_loop"].isNull()) {
		visible_shape = value["particle3d_loop"].asBool();
	}

	if (!value["linear_filter"].isNull()) {
		linear_filter = value["linear_filter"].asBool();
	}
}

}