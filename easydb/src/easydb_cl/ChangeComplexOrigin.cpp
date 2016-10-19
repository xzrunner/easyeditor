#include "ChangeComplexOrigin.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>
#include <ee/StringHelper.h>

#include <sprite2/SymType.h>

#include <fstream>

namespace edb
{

ChangeComplexOrigin::ChangeComplexOrigin()
{
// 	m_items.push_back(Item("bamboo_02_complex.json", -68.1f, -186.7f));
// 	m_items.push_back(Item("bamboo_03_complex.json", -73, -196.1f));
// 	m_items.push_back(Item("bamboo_04_complex.json", -102, -212.7f));
// 	m_items.push_back(Item("bamboo_05_complex.json", -122.8f, -121.9f));
// 	m_items.push_back(Item("bamboo_06_complex.json", -153.9f, -119.3f));
// 	m_items.push_back(Item("bamboo_07_complex.json", -77.8f, -142.7f));
// 	m_items.push_back(Item("bamboo_08_complex.json", -67.4f, -147.8f));

	m_items.push_back(Item("zu_zi_001_complex.json", -5.6f, -95.1f));
	m_items.push_back(Item("zu_zi_002_complex.json", -0.2f, -43.2f));
	m_items.push_back(Item("zu_zi_003_complex.json", -4.2f, -393.2f));
}

std::string ChangeComplexOrigin::Command() const
{
	return "change-origin";
}

std::string ChangeComplexOrigin::Description() const
{
	return "change complex origin";
}

std::string ChangeComplexOrigin::Usage() const
{
	return Command() + " [dir]";
}

int ChangeComplexOrigin::Run(int argc, char *argv[])
{
	// change-origin D:\projects\ejoy\coco-tools\sg_particle\data\json_and_pngs

	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}

void ChangeComplexOrigin::Trigger(const std::string& dir) const
{
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
		case s2::SYM_UNKNOWN:
			if (filepath.find("_lr.json") != std::string::npos) {
				FixLR(filepath);
			}
			break;
		}
	}
}

void ChangeComplexOrigin::FixComplex(const std::string& filepath) const
{
	const Item* item = QueryItem(filepath);
	if (!item) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	for (int i = 0, n = value["sprite"].size(); i < n; ++i) 
	{
		Json::Value& spr_val = value["sprite"][i];
		sm::vec2 pos;
		if (spr_val.isMember("position")) {
			pos.x = spr_val["position"]["x"].asDouble();
			pos.y = spr_val["position"]["y"].asDouble();
		}
		pos -= item->trans;
		spr_val["position"]["x"] = pos.x;
		spr_val["position"]["y"] = pos.y;
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void ChangeComplexOrigin::FixLR(const std::string& path) const
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(path);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	bool dirty = false;

	Json::Value& layer_val = value["layer"];
	for (int i = 0, n = layer_val.size(); i < n; ++i) {
		Json::Value& spr_val = layer_val[i]["sprite"];
		for (int j = 0, m = spr_val.size(); j < m; ++j) {
			std::string filepath = spr_val[j]["filepath"].asString();
			if (filepath == ee::SYM_GROUP_TAG) {
				Json::Value& group_val = spr_val[j][ee::SYM_GROUP_TAG];
				for (int i = 0, n = group_val.size(); i < n; ++i) {
					if (FixLRSpr(group_val[i])) {
						dirty = true;
					}
				}
			} else {
				if (FixLRSpr(spr_val[j])) {
					dirty = true;
				}
			}
		}
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

bool ChangeComplexOrigin::FixLRSpr(Json::Value& val) const
{
	std::string filepath = val["filepath"].asString();
	const Item* item = QueryItem(filepath);
	if (!item) {
		return false;
	}

	sm::vec2 pos;
	if (val.isMember("position")) {
		pos.x = val["position"]["x"].asDouble();
		pos.y = val["position"]["y"].asDouble();
	}
	pos += item->trans;
	val["position"]["x"] = pos.x;
	val["position"]["y"] = pos.y;

	return true;
}

const ChangeComplexOrigin::Item* ChangeComplexOrigin::QueryItem(const std::string& filepath) const
{
	std::string fmt_path = filepath;
	ee::StringHelper::ToLower(fmt_path);
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		if (fmt_path.find(m_items[i].filename) != std::string::npos) {
			return &m_items[i];
		}
	}
	return NULL;
}

}
