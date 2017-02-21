#include "ScaleOverall.h"
#include "check_params.h"
#include "utility.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/ImageSymbol.h>
#include <ee/SpriteIO.h>
#include <ee/SymbolFile.h>
#include <ee/Config.h>
#include <ee/SettingData.h>

#include <SM_Calc.h>
#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>
#include <gum/Config.h>

#include <fstream>

namespace edb
{

std::string ScaleOverall::Command() const
{
	return "scale";
}

std::string ScaleOverall::Description() const
{
	return "scale symbol include images and json file";
}

std::string ScaleOverall::Usage() const
{
	return Command() + " [dir path] [scale]";
}

int ScaleOverall::Run(int argc, char *argv[])
{
	// scale E:\test2\2000cunminnan 0.5f

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;

	gum::Config* cfg = gum::Config::Instance();
	bool ori_alpha_cfg = cfg->GetPreMulAlpha();
	cfg->SetPreMulAlpha(false);

	Scale(argv[2], atof(argv[3]));

	data.open_image_edge_clip = ori_clip_cfg;
	cfg->SetPreMulAlpha(ori_alpha_cfg);

	return 0;
}

void ScaleOverall::Scale(const std::string& dir, float scale) const
{	
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	std::map<std::string, sm::vec2> mapImg2Center;

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::SymbolFile::Instance()->Type(filepath) == s2::SYM_IMAGE) {
			ScaleImage(filepath, scale, mapImg2Center);
		}
	}

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX:
			ScaleComplex(filepath, scale, mapImg2Center);
			break;
		case s2::SYM_ANIMATION:
			ScaleAnim(filepath, scale, mapImg2Center);
			break;
		}
	}
}

void ScaleOverall::ScaleImage(const std::string& filepath, float scale,
							  std::map<std::string, sm::vec2>& mapImg2Center) const
{
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	
	ee::ImageSymbol* img = static_cast<ee::ImageSymbol*>(sym);

	sm::vec2 img_offset = img->GetBounding().Center();
	mapImg2Center.insert(std::make_pair(filepath, img_offset));

	s2::DrawRT rt;
	rt.Draw(sym);
	sm::vec2 sz = sym->GetBounding().Size();
	rt.StoreToFile(filepath, sz.x, sz.y);

	sym->RemoveReference();
}

void ScaleOverall::ScaleComplex(const std::string& path, float scale,
								const std::map<std::string, sm::vec2>& mapImg2Center) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(path);

	int i = 0;
	Json::Value spriteVal = value["sprite"][i++];
	while (!spriteVal.isNull()) {
		std::string relative = spriteVal["filepath"].asString();
		std::string filepath = ee::FileHelper::GetAbsolutePath(dir, relative);

		std::map<std::string, sm::vec2>::const_iterator itr 
			= mapImg2Center.find(filepath);
		if (itr == mapImg2Center.end()) {
//			throw ee::Exception("Image %s, not found in images!", filepath.c_str());
			continue;
		} else {
			sm::vec2 pos = GetScaledPos(spriteVal, scale, itr->second);
			value["sprite"][i-1]["position"]["x"] = pos.x;
			value["sprite"][i-1]["position"]["y"] = pos.y;
		}
		
		spriteVal = value["sprite"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void ScaleOverall::ScaleAnim(const std::string& path, float scale,
							 const std::map<std::string, sm::vec2>& mapImg2Center) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(path);

	int i = 0;
	Json::Value layerVal = value["layer"][i++];
	while (!layerVal.isNull()) {
		int j = 0;
		Json::Value frameVal = layerVal["frame"][j++];
		while (!frameVal.isNull()) {
			int k = 0;
			Json::Value entryVal = frameVal["actor"][k++];
			while (!entryVal.isNull()) {
				std::string relative = entryVal["filepath"].asString();
				std::string filepath = ee::FileHelper::GetAbsolutePath(dir, relative);

				std::map<std::string, sm::vec2>::const_iterator itr 
					= mapImg2Center.find(filepath);
				if (itr == mapImg2Center.end()) {
//					throw ee::Exception("Image %s, not found in images!", filepath.c_str());
					continue;
				} else {
					sm::vec2 pos = GetScaledPos(entryVal, scale, itr->second);
					value["layer"][i-1]["frame"][j-1]["actor"][k-1]["position"]["x"] = pos.x;
					value["layer"][i-1]["frame"][j-1]["actor"][k-1]["position"]["y"] = pos.y;
				}

				entryVal = frameVal["actor"][k++];
			}

			frameVal = layerVal["frame"][j++];
		}

		layerVal = value["layer"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

sm::vec2 ScaleOverall::GetScaledPos(Json::Value& sprite_val, float scale, 
									const sm::vec2& img_offset) const
{
	ee::SpriteIO spr_io;
	std::string dir = ee::FileHelper::GetFileDir(sprite_val["filepath"].asString());
	spr_io.Load(sprite_val, dir);

// 	sm::vec2 center = pos + sm::rotate_vector(-offset, angle);
// 	center = center + sm::rotate_vector(img_offset, angle);

	sm::vec2 center = spr_io.m_position + sm::rotate_vector(img_offset, spr_io.m_angle);

	center *= scale;

	return center;
}

}