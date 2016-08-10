#include "MTConfigMgr.h"
#include "FileIO.h"
#include "mt_config.h"

#include <mt_2d.h>

namespace etrail
{

MTConfigMgr* MTConfigMgr::m_instance = NULL;

MTConfigMgr::MTConfigMgr()
{
}

t2d_emitter_cfg* MTConfigMgr::GetDefaultConfig()
{
	const char* KEY = "default";
	t2d_emitter_cfg* cfg = NULL;
	std::map<std::string, t2d_emitter_cfg*>::iterator itr = m_map2cfg.find(KEY);
	int sz = SIZEOF_T2D_EMITTER_CFG + SIZEOF_T2D_SYMBOL * MAX_COMPONENTS;
	if (itr == m_map2cfg.end()) {
		cfg = (t2d_emitter_cfg*) operator new(sz);
		m_map2cfg.insert(std::make_pair(KEY, cfg));
	} else {
		cfg = itr->second;
	}
	memset(cfg, 0, sz);
	cfg->symbols = (t2d_symbol*)(cfg+1);
	cfg->mode_type = T2D_MODE_IMAGE;
	return cfg;
}

t2d_emitter_cfg* MTConfigMgr::GetConfig(const std::string& filepath)
{
	std::map<std::string, t2d_emitter_cfg*>::iterator itr = m_map2cfg.find(filepath);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	} else {
		t2d_emitter_cfg* cfg = FileIO::LoadMTConfig(filepath);
		m_map2cfg.insert(std::make_pair(filepath, cfg));
		return cfg;
	}
}

std::string MTConfigMgr::GetFilepath(const t2d_emitter_cfg* cfg)
{
	std::map<std::string, t2d_emitter_cfg*>::iterator itr = m_map2cfg.begin();
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