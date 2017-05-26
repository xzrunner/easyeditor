#include "Symbol.h"
#include "MTConfigMgr.h"

#include <mt_2d.h>
#include <sprite2/Trail.h>
#include <sprite2/TrailEmitterCfg.h>
#include <sprite2/TrailEmitter.h>
#include <gum/FilepathHelper.h>

#include <json/json.h>

#include <fstream>

namespace etrail
{

Symbol::Symbol()
{
	s2::Trail::Instance();
}

sm::rect Symbol::GetBoundingImpl(const s2::Sprite* spr, const s2::Actor* actor, bool cache) const
{
	return sm::rect(200, 200);
}

bool Symbol::LoadResources()
{
	if (!gum::FilepathHelper::Exists(m_filepath)) {
		return false;
	}

	m_et_cfg = MTConfigMgr::Instance()->GetConfig(m_filepath);
	m_et = s2::TrailEmitterPool::Instance()->Pop();
	m_et->CreateEmitter(m_et_cfg);
	m_et->Start();

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