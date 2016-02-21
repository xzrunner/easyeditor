#include "LoadFromLua.h"
#include "NodeToSprite.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/Image.h>

namespace ecomplex
{

void LoadFromLua::Load(const Json::Value& value, const std::string& dir, 
					   Symbol* symbol)
{
	std::vector<ee::Image*> images;
	std::string img_name = ee::FileHelper::GetAbsolutePath(dir, value["image name"].asString());
	LoadImages(img_name, images);

	std::string lua_file = ee::FileHelper::GetAbsolutePath(dir, value["lua desc"].asString());
	erespacker::ResUnpacker unpacker;
	unpacker.UnpackLua(lua_file, images);

	std::string export_name = value["export name"].asString();

	erespacker::IPackNode* node = erespacker::UnpackNodeFactory::Instance()->Query(export_name);
	symbol->m_sprites.push_back(NodeToSprite::Trans(node));
}

void LoadFromLua::LoadImages(const std::string& name, std::vector<ee::Image*>& images)
{
	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool old_open_image_edge_clip = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool old_pre_multi_alpha = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	int idx = 1;
	while (true)
	{
		std::string filepath = name + ee::StringHelper::ToString(idx++) + ".png";
		if (ee::FileHelper::IsFileExist(filepath)) {
			ee::Image* img = ee::ImageMgr::Instance()->GetItem(filepath);
			images.push_back(img);
		} else {	
			break;
		}
	}

	data.open_image_edge_clip = old_open_image_edge_clip;
	data.pre_multi_alpha = old_pre_multi_alpha;
}

}