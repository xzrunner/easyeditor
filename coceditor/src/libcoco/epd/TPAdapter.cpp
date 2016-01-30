#include "TPAdapter.h"

#include <fstream>

#include <drag2d.h>

namespace libcoco
{
namespace epd
{

void TPAdapter::Load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_width = value["meta"]["size"]["w"].asInt();
	m_height = value["meta"]["size"]["h"].asInt();

	std::string scale = value["meta"]["scale"].asString();
	m_invscale = atof(scale.c_str());

	std::string app = value["meta"]["app"].asString();
	m_is_easydb = app.compare("easydb") == 0;

	int i = 0;
	Json::Value frameValue = value["frames"][i++];
	while (!frameValue.isNull()) {
		Entry entry;
		Load(frameValue, entry);
		m_frames.push_back(entry);
		frameValue = value["frames"][i++];
	}
}

void TPAdapter::Load(const Json::Value& value, Entry& entry)
{
	std::string filepath = value["filename"].asString();

	wxFileName filename(filepath);
	filename.MakeAbsolute(m_src_data_dir);
	filename.Normalize();
	filepath = d2d::FileHelper::FormatFilepath(filename.GetFullPath().ToStdString());

	entry.filename = filepath;

	Load(value["frame"], entry.frame);
	entry.rotated = value["rotated"].asBool();
	entry.trimmed = value["trimmed"].asBool();
	Load(value["spriteSourceSize"], entry.sprite_source_size);
	entry.src_width = value["sourceSize"]["w"].asInt();
	entry.src_height = value["sourceSize"]["h"].asInt();
}

void TPAdapter::Load(const Json::Value& value, Region& region)
{
	region.x = value["x"].asInt();
	region.y = value["y"].asInt();
	region.w = value["w"].asInt();
	region.h = value["h"].asInt();
}

}
}