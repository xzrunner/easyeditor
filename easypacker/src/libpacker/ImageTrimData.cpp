#include "ImageTrimData.h"

#include <json.h>
#include <fstream>

namespace libpacker
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

		std::string filepath = spr_val["filepath"].asString();
		m_trim_info.insert(std::make_pair(filepath, trim));

		spr_val = value[i++];
	}
}

}