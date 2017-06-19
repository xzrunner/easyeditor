#include "JsonResFixOP.h"

namespace edb
{

JsonResFixOP::JsonResFixOP(const std::string& filepath)
	: JsonResOP(filepath)
{
}

bool JsonResFixOP::OnDoFile(const std::string& filepath, Json::Value& val, const std::string& key)
{
	return filepath.empty();
}

void JsonResFixOP::AfterDoGroup(bool dirty, Json::Value& val)
{
	if (!dirty) {
		return;
	}

	Json::Value new_group;
	for (int i = 0, n = val["group"].size(); i < n; ++i) {
		if (!val["group"][i]["filepath"].isNull()) {
			new_group[new_group.size()] = val["group"][i];
		} 
	}
	val["group"] = new_group;
}

void JsonResFixOP::AfterDoCommon(bool dirty, Json::Value& val, const std::string& key)
{
	if (!dirty) {
		return;
	}

	Json::Value new_key;
	for (int i = 0, n = val[key].size(); i < n; ++i) {
		if (!val[key][i][JsonResOP::KEY_PATH].isNull()) {
			new_key[new_key.size()] = val[key][i];
		} 
	}
	val[key] = new_key;
}

void JsonResFixOP::AfterDoTexture(bool dirty, Json::Value& val)
{
	if (!dirty) {
		return;
	}

	Json::Value new_shapes;
	for (int i = 0, n = val["shapes"].size(); i < n; ++i) 
	{
		if (val["shapes"][i].isMember("material")) {
			Json::Value& val_mat = val["shapes"][i]["material"];
			if (val_mat["type"] == "texture" && val_mat["texture path"].isNull()) {
				;
			} else {
				new_shapes[new_shapes.size()] = val["shapes"][i];
			}
		} else {
			new_shapes[new_shapes.size()] = val["shapes"][i];
		}
	}
	val["shapes"] = new_shapes;
}

void JsonResFixOP::AfterDoAnim(bool dirty, Json::Value& val)
{
	if (!dirty) {
		return;
	}

	for (int layer_i = 0, layer_n = val["layer"].size(); layer_i < layer_n; ++layer_i) {
		Json::Value& layer_val = val["layer"][layer_i];
		for (int frame_i = 0, frame_n = layer_val["frame"].size(); frame_i < frame_n; ++frame_i) {
			Json::Value& frame_val = layer_val["frame"][frame_i];
			Json::Value new_actors;
			for (int actor_i = 0, actor_n = frame_val["actor"].size(); actor_i < actor_n; ++actor_i) {
				if (!frame_val["actor"][actor_i][KEY_PATH].isNull()) {
					new_actors[new_actors.size()] = frame_val["actor"][actor_i];
				}
			}
			frame_val["actor"] = new_actors;
		}
	}
}

}