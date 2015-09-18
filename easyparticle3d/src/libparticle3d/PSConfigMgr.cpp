#include "PSConfigMgr.h"
#include "FileIO.h"
#include "ps_config.h"

#include <particle3d.h>

namespace eparticle3d
{

PSConfigMgr* PSConfigMgr::m_instance = NULL;

PSConfigMgr::PSConfigMgr()
{
}

p3d_ps_config* PSConfigMgr::GetDefaultConfig()
{
	const char* KEY = "default";
	p3d_ps_config* cfg = NULL;
	std::map<std::string, p3d_ps_config*>::iterator itr = m_map2cfg.find(KEY);
	int sz = SIZEOF_P3D_PS_CONFIG + SIZEOF_P3D_SYMBOL * MAX_COMPONENTS;
	if (itr == m_map2cfg.end()) {
		cfg = (p3d_ps_config*) operator new(sz);
		m_map2cfg.insert(std::make_pair(KEY, cfg));
	} else {
		cfg = itr->second;
	}
	memset(cfg, 0, sz);
	cfg->symbols = (p3d_symbol*)(cfg+1);
	return cfg;
}

p3d_ps_config* PSConfigMgr::GetConfig(const std::string& filepath)
{
	std::map<std::string, p3d_ps_config*>::iterator itr = m_map2cfg.find(filepath);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	} else {
		p3d_ps_config* cfg = FileIO::LoadPSConfig(filepath);
		m_map2cfg.insert(std::make_pair(filepath, cfg));
		return cfg;
	}
}

std::string PSConfigMgr::GetFilepath(const p3d_ps_config* cfg)
{
	std::map<std::string, p3d_ps_config*>::iterator itr = m_map2cfg.begin();
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