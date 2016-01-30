#include "ImageTrimData.h"

#include <drag2d.h>

#include <json/json.h>
#include <fstream>

namespace libtexpacker
{

ImageTrimData::ImageTrimData(const std::string& filepath)
{
	Load(filepath);
}

const ImageTrimData::Trim* ImageTrimData::Query(const std::string& filepath) const
{
	std::map<std::string, Trim>::const_iterator itr 
		= m_trim_info.find(filepath);
	if (itr != m_trim_info.end()) {
		return &itr->second;
	} else {
		return NULL;
	}
}

void ImageTrimData::Load(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FileHelper::GetFileDir(filepath);

	int i = 0;
	Json::Value spr_val = value[i++];
	while (!spr_val.isNull()) {
		Trim trim;
		trim.x = spr_val["position"]["x"].asInt();
		trim.y = spr_val["position"]["y"].asInt();
		trim.w = spr_val["position"]["w"].asInt();
		trim.h = spr_val["position"]["h"].asInt();
		trim.ori_w = spr_val["source size"]["w"].asInt();
		trim.ori_h = spr_val["source size"]["h"].asInt();

		trim.bound[0] = spr_val["bound"]["left"]["max"].asInt();
		trim.bound[1] = spr_val["bound"]["left"]["tot"].asInt();
		trim.bound[2] = spr_val["bound"]["bottom"]["max"].asInt();
		trim.bound[3] = spr_val["bound"]["bottom"]["tot"].asInt();
		trim.bound[4] = spr_val["bound"]["right"]["max"].asInt();
		trim.bound[5] = spr_val["bound"]["right"]["tot"].asInt();
		trim.bound[6] = spr_val["bound"]["up"]["max"].asInt();
		trim.bound[7] = spr_val["bound"]["up"]["tot"].asInt();

		std::string file_path = dir + "\\" + spr_val["filepath"].asString();
		file_path = d2d::FileHelper::FormatFilepathAbsolute(file_path);

		m_trim_info.insert(std::make_pair(file_path, trim));

		spr_val = value[i++];
	}
}

}