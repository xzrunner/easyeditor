#include <gl/glew.h>

#include "ScaleOverall.h"
#include "check_params.h"

#include <ee/ShaderMgr.h>
#include <ee/Snapshoot.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/Config.h>
#include <ee/SymbolMgr.h>
#include <ee/ImageSymbol.h>
#include <ee/Math2D.h>

#include <glfw.h>

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

void ScaleOverall::Run(int argc, char *argv[])
{
	// scale E:\test2\2000cunminnan 0.5f

	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;

	glfwInit();
	if(!glfwOpenWindow(100, 100, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}

	if (glewInit() != GLEW_OK) {
		return;
	}

	ee::ShaderMgr::Instance()->reload();

	ee::Snapshoot ss;

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	Scale(ss, argv[2], atof(argv[3]));

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;
}

void ScaleOverall::Scale(ee::Snapshoot& ss, const std::string& dir, float scale) const
{	
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	std::map<std::string, ee::Vector> mapImg2Center;

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_image)) {
			ScaleImage(filepath, scale, ss, mapImg2Center);
		}
	}

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			ScaleComplex(filepath, scale, mapImg2Center);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			ScaleAnim(filepath, scale, mapImg2Center);
		}
	}
}

void ScaleOverall::ScaleImage(const std::string& filepath, float scale, ee::Snapshoot& ss,
							  std::map<std::string, ee::Vector>& mapImg2Center) const
{
	ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	
	ee::ImageSymbol* img = static_cast<ee::ImageSymbol*>(symbol);

	ee::Vector img_offset;
	img_offset.x = img->GetSize().CenterX();
	img_offset.y = img->GetSize().CenterY();
	mapImg2Center.insert(std::make_pair(filepath, img_offset));

	ss.OutputToImageFile(symbol, filepath, scale);
	symbol->Release();
}

void ScaleOverall::ScaleComplex(const std::string& path, float scale,
								const std::map<std::string, ee::Vector>& mapImg2Center) const
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

		std::map<std::string, ee::Vector>::const_iterator itr 
			= mapImg2Center.find(filepath);
		if (itr == mapImg2Center.end()) {
//			throw ee::Exception("Image %s, not found in images!", filepath.c_str());
			continue;
		} else {
			ee::Vector pos = GetScaledPos(spriteVal, scale, itr->second);
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
							 const std::map<std::string, ee::Vector>& mapImg2Center) const
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

				std::map<std::string, ee::Vector>::const_iterator itr 
					= mapImg2Center.find(filepath);
				if (itr == mapImg2Center.end()) {
//					throw ee::Exception("Image %s, not found in images!", filepath.c_str());
					continue;
				} else {
					ee::Vector pos = GetScaledPos(entryVal, scale, itr->second);
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

ee::Vector ScaleOverall::GetScaledPos(Json::Value& sprite_val, float scale, 
									   const ee::Vector& img_offset) const
{
	ee::Vector pos;
	pos.x = sprite_val["position"]["x"].asDouble();
	pos.y = sprite_val["position"]["y"].asDouble();

	float angle = sprite_val["angle"].asDouble();

	ee::Vector offset;
	offset.x = sprite_val["x offset"].asDouble();
	offset.y = sprite_val["y offset"].asDouble();

// 	ee::Vector center = pos + ee::Math2D::RotateVector(-offset, angle);
// 	center = center + ee::Math2D::RotateVector(img_offset, angle);

	ee::Vector center = pos + ee::Math2D::RotateVector(img_offset, angle);

	center *= scale;

	return center;
}

}