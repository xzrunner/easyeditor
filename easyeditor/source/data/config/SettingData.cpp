#include "SettingData.h"

namespace ee
{

int SettingData::ctl_pos_sz = 0;
bool SettingData::draw_tris_edge = false;
bool SettingData::draw_poly_bound = true;

SettingData::DrawType SettingData::draw_type = e_DrawBoth;

SettingData::SettingData()
{
	open_sprite_capture = true;
	auto_get_focus = true;

	open_image_edge_clip = true;
	visible_image_edge = false;

	visible_label_bg = true;
	visible_label_text = true;
	visible_node_name = false;
	node_name_scale = 1.0f;
	
	visible_shape = true;
	visible_tex_edge = true;
	particle3d_loop = true;
	bg_color.Set(0.5f, 0.5f, 0.5f);
	visible_spr_update = true;

	img_list_compact = false;

	linear_filter = true;

	pre_multi_alpha = true;
	load_image = true;

	color_setting_dlg_type = CSDT_DEFAULT;

	orthogonal = true;
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
	if (!value["node_name_scale"].isNull()) {
		node_name_scale = static_cast<float>(value["node_name_scale"].asDouble());
	}
	if (!value["visible_node_name"].isNull()) {
		visible_node_name = value["visible_node_name"].asBool();
	}
	if (!value["visible_tex_edge"].isNull()) {
		visible_tex_edge = value["visible_tex_edge"].asBool();
	}
	if (!value["visible_shape"].isNull()) {
		visible_shape = value["visible_shape"].asBool();
	}
	if (!value["particle3d_loop"].isNull()) {
		particle3d_loop = value["particle3d_loop"].asBool();
	}
	if (!value["visible_spr_update"].isNull()) {
		visible_spr_update = value["visible_spr_update"].asBool();
	}

	if (!value["img_list_compact"].isNull()) {
		img_list_compact = value["img_list_compact"].asBool();
	}

	if (!value["linear_filter"].isNull()) {
		linear_filter = value["linear_filter"].asBool();
	}

	if (!value["color_setting_dlg"].isNull()) {
		std::string type = value["color_setting_dlg"].asString();
		if (type == "rgb") {
			color_setting_dlg_type = CSDT_RGB;
		} else if (type == "hsl") {
			color_setting_dlg_type = CSDT_HSL;
		}
	}
}

}