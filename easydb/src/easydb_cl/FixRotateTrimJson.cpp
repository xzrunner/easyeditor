#include "FixRotateTrimJson.h"
#include "RotateTrimImage.h"
#include "check_params.h"



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
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			FixComplex(filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			FixAnim(filepath);
		}
	}
}

bool FixRotateTrimJson::FixSprite(const wxString& filepath, Json::Value& sprite_val) const
{
	std::string spr_path = sprite_val["filepath"].asString();
	if (!ee::FileType::IsType(spr_path, ee::FileType::e_image)) {
		return false;
	}

	wxString spr_abs_path = ee::FileHelper::GetAbsolutePathFromFile(filepath, spr_path);
	wxString spr_rel_path = ee::FileHelper::GetRelativePath(m_dir, spr_abs_path);
	std::map<std::string, TrimInfo>::const_iterator itr 
		= m_trim_info.find(spr_rel_path.ToStdString());
	if (itr == m_trim_info.end()) {
		return false;
	}

	TrimInfo trim = itr->second;

	ee::Vector old_pos, old_offset;
	old_pos.x = sprite_val["position"]["x"].asDouble();
	old_pos.y = sprite_val["position"]["y"].asDouble();
	old_offset.x = sprite_val["x offset"].asDouble();
	old_offset.y = sprite_val["y offset"].asDouble();
	float old_angle = sprite_val["angle"].asDouble();

	ee::Vector new_offset = ee::Math2D::RotateVector(old_offset, old_angle);

	ee::Vector trim_offset = ee::Math2D::RotateVector(ee::Vector(trim.x, trim.y), -trim.angle);

	float new_angle = old_angle - trim.angle;

	ee::Vector new_pos = ee::Math2D::RotateVector(-old_offset, old_angle) + old_offset + old_pos 
		- ee::Math2D::RotateVector(-new_offset, new_angle) - new_offset - trim_offset;

	sprite_val["position"]["x"] = new_pos.x;
	sprite_val["position"]["y"] = new_pos.y;
	sprite_val["angle"] = new_angle;
	sprite_val["x offset"] = new_offset.x;
	sprite_val["y offset"] = new_offset.y;

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