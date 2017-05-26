#include "MTConfigMgr.h"
#include "FileIO.h"
#include "mt_config.h"

#include <mt_2d.h>
#include <sprite2/TrailEmitterCfg.h>

namespace etrail
{

MTConfigMgr* MTConfigMgr::m_instance = NULL;

MTConfigMgr::MTConfigMgr()
{
}

s2::TrailEmitterCfg* MTConfigMgr::GetDefaultConfig()
{
	const char* KEY = "default";
	std::map<std::string, s2::TrailEmitterCfg*>::iterator itr = m_map2cfg.find(KEY);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	}

	int sz = SIZEOF_T2D_EMITTER_CFG + SIZEOF_T2D_SYMBOL * MAX_COMPONENTS;
	t2d_emitter_cfg* cfg_impl = (t2d_emitter_cfg*) operator new(sz);
	memset(cfg_impl, 0, sz);
	cfg_impl->syms = (t2d_symbol*)(cfg_impl+1);
	cfg_impl->mode_type = T2D_MODE_IMAGE;

	s2::TrailEmitterCfg* cfg = new s2::TrailEmitterCfg(cfg_impl);
	m_map2cfg.insert(std::make_pair(KEY, cfg));

	return cfg;
}

s2::TrailEmitterCfg* MTConfigMgr::GetConfig(const std::string& filepath)
{
	std::map<std::string, s2::TrailEmitterCfg*>::iterator itr = m_map2cfg.find(filepath);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	} else {
		s2::TrailEmitterCfg* cfg = FileIO::LoadMTConfig(filepath);
		m_map2cfg.insert(std::make_pair(filepath, cfg));
		return cfg;
	}
}

std::string MTConfigMgr::GetFilepath(const s2::TrailEmitterCfg* cfg)
{
	std::map<std::string, s2::TrailEmitterCfg*>::iterator itr = m_map2cfg.begin();
	for ( ; itr != m_map2cfg.end(); ++itr) {
		if (cfg == itr->second) {
			return itr->first;
		}
	}
	return "";
}

MTConfigMgr* MTConfigMgr::Instance()
{
	if (!m_instance) {
		m_instance = new MTConfigMgr();
	}
	return m_instance;
}

}