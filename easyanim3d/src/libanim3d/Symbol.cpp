#include "Symbol.h"
#include "Sprite.h"

#include <ee/FileHelper.h>

#include <model3/ModelObj.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace eanim3d
{

Symbol::Symbol()
{
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	if (m_model) {
		delete m_model;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(m_filepath);
	std::string filepath = ee::FileHelper::GetAbsolutePath(dir, value["filepath"].asString());
	m_model = new m3::ModelObj(filepath.c_str(), 0.02f);

	return true;
}

}