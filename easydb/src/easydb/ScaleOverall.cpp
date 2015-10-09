#include <gl/glew.h>

#include "ScaleOverall.h"
#include "check_params.h"

#include <wx/filename.h>
#include <drag2d.h>
#include <glfw.h>

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

	d2d::ShaderMgr::Instance()->reload();

	d2d::Snapshoot ss;

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	Scale(ss, argv[2], atof(argv[3]));

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;
}

void ScaleOverall::Scale(d2d::Snapshoot& ss, const std::string& dir, float scale) const
{	
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);

	std::map<std::string, d2d::Vector> mapImg2Center;

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			ScaleImage(filepath, scale, ss, mapImg2Center);
		}
	}

	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			ScaleComplex(filepath, scale, mapImg2Center);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			ScaleAnim(filepath, scale, mapImg2Center);
		}
	}
}

void ScaleOverall::ScaleImage(const std::string& filepath, float scale, d2d::Snapshoot& ss,
							  std::map<std::string, d2d::Vector>& mapImg2Center) const
{
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	
	d2d::ImageSymbol* img = static_cast<d2d::ImageSymbol*>(symbol);

	d2d::Vector img_offset;
	img_offset.x = img->GetSize().xCenter();
	img_offset.y = img->GetSize().yCenter();
	mapImg2Center.insert(std::make_pair(filepath, img_offset));

	ss.OutputToImageFile(symbol, filepath, scale);
	symbol->Release();
}

void ScaleOverall::ScaleComplex(const std::string& path, float scale,
								const std::map<std::string, d2d::Vector>& mapImg2Center) const
{
	wxFileName filename(path);
	filename.Normalize();
	std::string filepath = filename.GetFullPath();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(path);

	int i = 0;
	Json::Value spriteVal = value["sprite"][i++];
	while (!spriteVal.isNull()) {
		std::string relative = spriteVal["filepath"].asString();
		std::string filepath = d2d::FilenameTools::getAbsolutePath(dir, relative);

		std::map<std::string, d2d::Vector>::const_iterator itr 
			= mapImg2Center.find(filepath);
		if (itr == mapImg2Center.end()) {
//			throw d2d::Exception("Image %s, not found in images!", filepath.c_str());
			continue;
		} else {
			d2d::Vector pos = GetScaledPos(spriteVal, scale, itr->second);
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
							 const std::map<std::string, d2d::Vector>& mapImg2Center) const
{
	wxFileName filename(path);
	filename.Normalize();
	std::string filepath = filename.GetFullPath();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(path);

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
				std::string filepath = d2d::FilenameTools::getAbsolutePath(dir, relative);

				std::map<std::string, d2d::Vector>::const_iterator itr 
					= mapImg2Center.find(filepath);
				if (itr == mapImg2Center.end()) {
//					throw d2d::Exception("Image %s, not found in images!", filepath.c_str());
					continue;
				} else {
					d2d::Vector pos = GetScaledPos(entryVal, scale, itr->second);
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

d2d::Vector ScaleOverall::GetScaledPos(Json::Value& sprite_val, float scale, 
									   const d2d::Vector& img_offset) const
{
	d2d::Vector pos;
	pos.x = sprite_val["position"]["x"].asDouble();
	pos.y = sprite_val["position"]["y"].asDouble();

	float angle = sprite_val["angle"].asDouble();

	d2d::Vector offset;
	offset.x = sprite_val["x offset"].asDouble();
	offset.y = sprite_val["y offset"].asDouble();

// 	d2d::Vector center = pos + d2d::Math::rotateVector(-offset, angle);
// 	center = center + d2d::Math::rotateVector(img_offset, angle);

	d2d::Vector center = pos + d2d::Math::rotateVector(img_offset, angle);

	center *= scale;

	return center;
}

}