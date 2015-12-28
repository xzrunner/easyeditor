#include "PSConfigMgr.h"
#include "ps_config.h"
#include "FileIO.h"

#include <ps_2d.h>

namespace eparticle2d
{

PSConfigMgr* PSConfigMgr::m_instance = NULL;

PSConfigMgr::PSConfigMgr()
{
}

p2d_emitter_cfg* PSConfigMgr::GetDefaultConfig()
{
	const char* KEY = "default";
	p2d_emitter_cfg* cfg = NULL;
	std::map<std::string, p2d_emitter_cfg*>::iterator itr = m_map2cfg.find(KEY);
	int sz = SIZEOF_P2D_PS_CONFIG + SIZEOF_P2D_SYMBOL * MAX_COMPONENTS;
	if (itr == m_map2cfg.end()) {
		cfg = (p2d_emitter_cfg*) operator new(sz);
		m_map2cfg.insert(std::make_pair(KEY, cfg));
	} else {
		cfg = itr->second;
	}
	memset(cfg, 0, sz);
	cfg->symbols = (p2d_symbol*)(cfg+1);
	return cfg;
}

p2d_emitter_cfg* PSConfigMgr::GetConfig(const std::string& filepath)
{
	std::map<std::string, p2d_emitter_cfg*>::iterator itr = m_map2cfg.find(filepath);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	} else {
		p2d_emitter_cfg* cfg = FileIO::LoadPSConfig(filepath);
		m_map2cfg.insert(std::make_pair(filepath, cfg));
		return cfg;
	}
}

std::string PSConfigMgr::GetFilepath(const p2d_emitter_cfg* cfg)
{
	std::map<std::string, p2d_emitter_cfg*>::iterator itr = m_map2cfg.begin();
	for ( ; itr != m_map2cfg.end(); ++itr) {
		if (cfg == itr->second) {
			return itr->first;
		}
	}
	return "";
}

PSConfigMgr* PSConfigMgr::Instance()
{
	if (!m_instance) {
		m_instance = new PSConfigMgr();
	}
	return m_instance;
}

}