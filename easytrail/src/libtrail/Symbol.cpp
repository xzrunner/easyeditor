#include "Symbol.h"
#include "MTConfigMgr.h"

#include <mt_2d.h>
#include <sprite2/Trail.h>
#include <gum/FilepathHelper.h>

#include <json/json.h>

#include <fstream>

namespace etrail
{

Symbol::Symbol()
{
	s2::Trail::Instance();
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	m_et_cfg = MTConfigMgr::Instance()->GetConfig(m_filepath);
	m_et = t2d_emitter_create(m_et_cfg);

	t2d_emitter_start(m_et);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	return true;
}

}