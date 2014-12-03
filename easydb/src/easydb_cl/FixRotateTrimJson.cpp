#include "FixRotateTrimJson.h"
#include "RotateTrimImage.h"
#include "check_params.h"

#include <drag2d.h>

namespace edb
{

std::string FixRotateTrimJson::Command() const
{
	return "fix-rotate-trim";
}

std::string FixRotateTrimJson::Description() const
{
	return "fix json file after image rotate-trim";
}

std::string FixRotateTrimJson::Usage() const
{
	return Command() + " [dir]";
}

void FixRotateTrimJson::Run(int argc, char *argv[])
{
	// fix-rotate-trim D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	Trigger(argv[2]);
}

void FixRotateTrimJson::Trigger(const std::string& dir)
{
	m_dir = dir;

	LoadTrimInfo(dir);

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			FixComplex(filepath);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			FixAnim(filepath);
		}
	}
}

bool FixRotateTrimJson::FixSprite(const wxString& filepath, Json::Value& sprite_val) const
{
	std::string spr_path = sprite_val["filepath"].asString();
	if (!d2d::FileNameParser::isType(spr_path, d2d::FileNameParser::e_image)) {
		return false;
	}

	wxString spr_abs_path = d2d::FilenameTools::getAbsolutePathFromFile(filepath, spr_path);
	wxString spr_rel_path = d2d::FilenameTools::getRelativePath(m_dir, spr_abs_path);
	std::map<std::string, TrimInfo>::const_iterator itr 
		= m_trim_info.find(spr_rel_path.ToStdString());
	if (itr == m_trim_info.end()) {
		return false;
	}

	TrimInfo trim = itr->second;

	d2d::Vector offset = d2d::Math::rotateVector(d2d::Vector(trim.x, trim.y), -trim.angle);
	sprite_val["position"]["x"] = -offset.x;
	sprite_val["position"]["y"] = -offset.y;
	sprite_val["angle"] = sprite_val["angle"].asDouble() - trim.angle;

	return true;
}

void FixRotateTrimJson::LoadTrimInfo(const std::string& dir)
{
	std::string trim_file = dir + "\\" + RotateTrimImage::GetOutputFileName();
	std::ifstream fin(trim_file.c_str(), std::ios::binary);
	if (!fin) {
		std::cout << "Can't open trim file. \n";
		return;
	}

	std::string line;
	while (std::getline(fin, line))
	{
		std::stringstream ss(line);

		std::string file;
		TrimInfo info;
		ss >> file >> info.x >> info.y >> info.angle;
		m_trim_info.insert(std::make_pair(file, info));
	}

	fin.close();
}

}