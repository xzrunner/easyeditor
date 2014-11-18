#include "SettingData.h"

namespace d2d
{

SettingData::SettingData()
{
	open_sprite_capture = true;

	open_image_edge_clip = true;
	visible_image_edge = false;

	visible_label_bg = true;
	visible_label_text = true;
	visible_node_name = false;

	linear_filter = true;
}

void SettingData::LoadFromFile(const Json::Value& value)
{
	open_sprite_capture = value["sprite_capture"].asBool();

	open_image_edge_clip = value["image_edge_clip"].asBool();
	visible_image_edge = value["visible_image_edge"].asBool();

	visible_label_bg = value["visible_label_bg"].asBool();
	visible_label_text = value["visible_label_text"].asBool();
	visible_node_name = value["visible_node_name"].asBool();

	linear_filter = value["linear_filter"].asBool();
}

}