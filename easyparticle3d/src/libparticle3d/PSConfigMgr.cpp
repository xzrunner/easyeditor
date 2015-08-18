#include "PSConfigMgr.h"
#include "FileIO.h"
#include "ps_config.h"

#include <ps/particle3d.h>

namespace eparticle3d
{

PSConfigMgr* PSConfigMgr::m_instance = NULL;

PSConfigMgr::PSConfigMgr()
{
}

ps_cfg_3d* PSConfigMgr::GetDefaultConfig()
{
	const char* KEY = "default";
	ps_cfg_3d* cfg = NULL;
	std::map<std::string, ps_cfg_3d*>::iterator itr = m_map2cfg.find(KEY);
	int sz = sizeof(ps_cfg_3d) + sizeof(particle_symbol) * MAX_COMPONENTS;
	if (itr == m_map2cfg.end()) {
		cfg = (ps_cfg_3d*) operator new(sz);
		m_map2cfg.insert(std::make_pair(KEY, cfg));
	} else {
		cfg = itr->second;
	}
	memset(cfg, 0, sz);
	cfg->symbols = (particle_symbol*)(cfg+1);
	return cfg;
}

ps_cfg_3d* PSConfigMgr::GetConfig(const std::string& filepath)
{
	std::map<std::string, ps_cfg_3d*>::iterator itr = m_map2cfg.find(filepath);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	} else {
		ps_cfg_3d* cfg = FileIO::LoadPSConfig(filepath);
		m_map2cfg.insert(std::make_pair(filepath, cfg));
		return cfg;
	}
}

std::string PSConfigMgr::GetFilepath(const ps_cfg_3d* cfg)
{
	std::map<std::string, ps_cfg_3d*>::iterator itr = m_map2cfg.begin();
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