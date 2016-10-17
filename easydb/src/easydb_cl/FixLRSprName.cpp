#include "FixLRSprName.h"
#include "check_params.h"

#include <ee/FileHelper.h>

#include <SM_Vector.h>
#include <sprite2/Color.h>
#include <gum/trans_color.h>

#include <fstream>

namespace edb
{

std::string FixLRSprName::Command() const
{
	return "fix-lr-name";
}

std::string FixLRSprName::Description() const
{
	return "fix LR sprite name";
}

std::string FixLRSprName::Usage() const
{
	return Command() + " [dir]";
}

int FixLRSprName::Run(int argc, char *argv[])
{
	// fix-lr-name D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	InitMap();

	Trigger(argv[2]);

	return 0;
}

bool FixLRSprName::FixSprite(Json::Value& val) const
{
	bool ret = false;

	std::string filepath = val["filepath"].asString();

	if (filepath == "group") 
	{
		Json::Value& group_val = val["group"];
		for (int i = 0, n = group_val.size(); i < n; ++i) {
			if (FixSprite(group_val[i])) {
				ret = true;
			}
		}
		return ret;
	}

	for (int i = 0, n = m_map2path.size(); i < n; ++i) 
	{
		int pos = filepath.find(m_map2path[i].first);
		if (pos == std::string::npos) {
			continue;
		}
		const std::string& src = m_map2path[i].first;
		const std::string& dst = m_map2path[i].second;
		filepath = filepath.substr(0, pos) + dst;
		val["filepath"] = filepath;
		ret = true;
		break;
	}

	for (int i = 0, n = m_map2name.size(); i < n; ++i) {
		if (filepath.find(m_map2name[i].first) != std::string::npos) {
			val["name"] = m_map2name[i].second;
			ret = true;
			break;
		}
	}

	return ret;
}

void FixLRSprName::InitMap()
{
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_hang_01.png", "\\res_mix\\corpse_hang_01_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_01.png", "\\res_mix\\corpse_lay_01_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_02.png", "\\res_mix\\corpse_lay_02_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_03.png", "\\res_mix\\corpse_lay_03_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_04.png", "\\res_mix\\corpse_lay_04_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_05.png", "\\res_mix\\corpse_lay_05_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_06.png", "\\res_mix\\corpse_lay_06_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_07.png", "\\res_mix\\corpse_lay_07_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_lay_08.png", "\\res_mix\\corpse_lay_08_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_sit_01.png", "\\res_mix\\corpse_sit_01_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_water_01.png", "\\res_mix\\corpse_water_01_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_water_02.png", "\\res_mix\\corpse_water_02_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\scene_blood_01.png", "\\res_mix\\scene_blood_01_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\scene_blood_02.png", "\\res_mix\\scene_blood_02_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\scene_blood_03.png", "\\res_mix\\scene_blood_03_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\scene_blood_04.png", "\\res_mix\\scene_blood_04_complex.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_drown_01.png", "\\res_mix\\corpse_drown_01_anim.json"));
	m_map2path.push_back(std::make_pair("\\res_png\\corpse_drown_02.png", "\\res_mix\\corpse_drown_02_anim.json"));

	m_map2name.push_back(std::make_pair("corpse_hang_01_complex.json", "corpse_hang_01"));
	m_map2name.push_back(std::make_pair("corpse_lay_01_complex.json", "corpse_lay_01"));
	m_map2name.push_back(std::make_pair("corpse_lay_02_complex.json", "corpse_lay_02"));
	m_map2name.push_back(std::make_pair("corpse_lay_03_complex.json", "corpse_lay_03"));
	m_map2name.push_back(std::make_pair("corpse_lay_04_complex.json", "corpse_lay_04"));
	m_map2name.push_back(std::make_pair("corpse_lay_05_complex.json", "corpse_lay_05"));
	m_map2name.push_back(std::make_pair("corpse_lay_06_complex.json", "corpse_lay_06"));
	m_map2name.push_back(std::make_pair("corpse_lay_07_complex.json", "corpse_lay_07"));
	m_map2name.push_back(std::make_pair("corpse_lay_08_complex.json", "corpse_lay_08"));
	m_map2name.push_back(std::make_pair("corpse_sit_01_complex.json", "corpse_sit_01"));
	m_map2name.push_back(std::make_pair("corpse_water_01_complex.json", "corpse_water_01"));
	m_map2name.push_back(std::make_pair("corpse_water_02_complex.json", "corpse_water_02"));
	m_map2name.push_back(std::make_pair("scene_blood_01_complex.json", "scene_blood_01"));
	m_map2name.push_back(std::make_pair("scene_blood_02_complex.json", "scene_blood_02"));
	m_map2name.push_back(std::make_pair("scene_blood_03_complex.json", "scene_blood_03"));
	m_map2name.push_back(std::make_pair("scene_blood_04_complex.json", "scene_blood_04"));
	m_map2name.push_back(std::make_pair("corpse_drown_01_anim.json", "corpse_drown_01"));
	m_map2name.push_back(std::make_pair("corpse_drown_02_anim.json", "corpse_drown_02"));
}

void FixLRSprName::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (filepath.find("_lr.json") == std::string::npos) {
			continue;
		}

		Json::Value val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, val);
		fin.close();

		bool dirty = false;
		for (int i = 0, n = val["layer"].size(); i < n; ++i) {
			Json::Value& layer_val = val["layer"][i];
			for (int j = 0, m = layer_val["sprite"].size(); j < m; ++j) {
				if (FixSprite(layer_val["sprite"][j])) {
					dirty = true;
				}
			}
		}

		if (dirty) {
			Json::StyledStreamWriter writer;
			std::locale::global(std::locale(""));
			std::ofstream fout(filepath.c_str());
			std::locale::global(std::locale("C"));	
			writer.write(fout, val);
			fout.close();
		}
	}
}

}