#include "LoadFromLua.h"
#include "NodeToSprite.h"
#include "Symbol.h"

namespace ecomplex
{

void LoadFromLua::Load(const Json::Value& value, const std::string& dir, 
					   Symbol* symbol)
{
	std::vector<d2d::Image*> images;
	std::string img_name = d2d::FilenameTools::getAbsolutePath(dir, value["image name"].asString());
	LoadImages(img_name, images);

	std::string lua_file = d2d::FilenameTools::getAbsolutePath(dir, value["lua desc"].asString());
	librespacker::ResUnpacker unpacker;
	unpacker.UnpackLua(lua_file, images);

	std::string export_name = value["export name"].asString();

	librespacker::IPackNode* node = librespacker::UnpackNodeFactory::Instance()->Query(export_name);
	symbol->m_sprites.push_back(NodeToSprite::Trans(node));
}

void LoadFromLua::LoadImages(const std::string& name, std::vector<d2d::Image*>& images)
{
	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool old_open_image_edge_clip = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool old_pre_multi_alpha = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	int idx = 1;
	while (true)
	{
		std::string filepath = name + d2d::StringTools::ToString(idx++) + ".png";
		if (d2d::FilenameTools::IsFileExist(filepath)) {
			d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(filepath);
			images.push_back(img);
		} else {	
			break;
		}
	}

	data.open_image_edge_clip = old_open_image_edge_clip;
	data.pre_multi_alpha = old_pre_multi_alpha;
}

}