#include "FixRotateTrimJson.h"
#include "RotateTrimImage.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SpriteIO.h>
#include <ee/SymbolFile.h>

#include <SM_Calc.h>
#include <sprite2/SymType.h>

#include <fstream>
#include <sstream>

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

int FixRotateTrimJson::Run(int argc, char *argv[])
{
	// fix-rotate-trim D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

void FixRotateTrimJson::Trigger(const std::string& dir)
{
	m_dir = dir;

	LoadTrimInfo(dir);

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		int type = ee::SymbolFile::Instance()->Type(filepath);
		switch (type)
		{
		case s2::SYM_COMPLEX:
			FixComplex(filepath);
			break;
		case s2::SYM_ANIMATION:
			FixAnim(filepath);
			break;
		}
	}
}

bool FixRotateTrimJson::FixSprite(const std::string& filepath, Json::Value& sprite_val) const
{
	std::string spr_path = sprite_val["filepath"].asString();
	if (ee::SymbolFile::Instance()->Type(filepath) != s2::SYM_IMAGE) {
		return false;
	}

	std::string spr_abs_path = ee::FileHelper::GetAbsolutePathFromFile(filepath, spr_path);
	std::string spr_rel_path = ee::FileHelper::GetRelativePath(m_dir, spr_abs_path);
	std::map<std::string, TrimInfo>::const_iterator itr 
		= m_trim_info.find(spr_rel_path);
	if (itr == m_trim_info.end()) {
		return false;
	}

	TrimInfo trim = itr->second;

	ee::SpriteIO spr_io;
	std::string spr_dir = ee::FileHelper::GetFileDir(spr_path);
	spr_io.Load(sprite_val, spr_dir.c_str());

	sm::vec2 new_offset = sm::rotate_vector(spr_io.m_offset, spr_io.m_angle);

	sm::vec2 trim_offset = sm::rotate_vector(sm::vec2(trim.x, trim.y), -trim.angle);

	float new_angle = spr_io.m_angle - trim.angle;

	sm::vec2 new_pos = sm::rotate_vector(-spr_io.m_offset, spr_io.m_angle) + spr_io.m_offset + spr_io.m_position 
		- sm::rotate_vector(-new_offset, new_angle) - new_offset - trim_offset;

	spr_io.m_position = new_pos;
	spr_io.m_angle = new_angle;
	spr_io.m_offset = new_offset;

	spr_io.Store(sprite_val, spr_dir.c_str());

	return true;
}

void FixRotateTrimJson::LoadTrimInfo(const std::string& dir)
{
	std::string trim_file = dir + "\\" + RotateTrimImage::GetOutputFileName();
	std::locale::global(std::locale(""));	
	std::ifstream fin(trim_file.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
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