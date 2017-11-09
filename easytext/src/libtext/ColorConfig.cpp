#include "ColorConfig.h"

#include <json/json.h>
#include <fstream>

namespace etext
{

static const char* FILEPATH = "font_color_cfg.json";

ColorConfig* ColorConfig::m_instance = NULL;

ColorConfig* ColorConfig::Instance()
{
	if (!m_instance) {
		m_instance = new ColorConfig();
	}
	return m_instance;
}

void ColorConfig::StoreToFile() const
{
	Json::Value val;

	val["color"] = m_color_data.ToString().ToStdString();

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(FILEPATH);
	std::locale::global(std::locale("C"));	
	writer.write(fout, val);
	fout.close();
}

void ColorConfig::LoadFromFile()
{
	Json::Value val;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(FILEPATH);
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	std::string color = val["color"].asString();

	m_color_data.FromString(color);
}

}