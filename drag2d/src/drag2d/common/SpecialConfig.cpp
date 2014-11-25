#include "SpecialConfig.h"

#include <drag2d.h>
//#include <fstream>

namespace d2d
{

SpecialConfig::SpecialConfig(const std::string& file_tag)
{
	m_filename = file_tag + "_config.json";
	Load();
}

SpecialConfig::~SpecialConfig()
{
	Store();
}

void SpecialConfig::Load()
{
	m_exist = true;
	if (!d2d::FilenameTools::isExist(m_filename)) {
		m_exist = false;
		return;
	}

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filename.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, m_value);
	fin.close();

	m_wnd.width = 800;
	m_wnd.height = 600;
	m_wnd.left = 0;
	m_wnd.top = 0;
	const Json::Value& wnd_val = m_value["window"];
	if (!wnd_val.isNull()) {
		if (!wnd_val["width"].isNull()) {
			m_wnd.width = wnd_val["width"].asInt();
		}
		if (!wnd_val["height"].isNull()) {
			m_wnd.height = wnd_val["height"].asInt();
		}
		if (!wnd_val["left"].isNull()) {
			m_wnd.left = wnd_val["left"].asInt();
		}
		if (!wnd_val["top"].isNull()) {
			m_wnd.top = wnd_val["top"].asInt();
		}
	}

	m_auto_set_wnd = false;
	if (!m_value["window_auto_set"].isNull()) {
		m_auto_set_wnd = m_value["window_auto_set"].asBool();
	}
}

void SpecialConfig::Store() const
{
	if (!m_exist || !m_auto_set_wnd) {
		return;
	}

	m_value["window"]["width"] = m_wnd.width;
	m_value["window"]["height"] = m_wnd.height;
	m_value["window"]["left"] = m_wnd.left;
	m_value["window"]["top"] = m_wnd.top;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(m_filename.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, m_value);
	fout.close();
}

}