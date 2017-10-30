#include "PSConfigMgr.h"
#include "FileIO.h"
#include "ps_config.h"

#include <ps_3d.h>
#include <sprite2/P3dEmitterCfg.h>

namespace eparticle3d
{

CU_SINGLETON_DEFINITION(PSConfigMgr)

PSConfigMgr::PSConfigMgr()
{
}

s2::P3dEmitterCfgPtr PSConfigMgr::GetDefaultConfig()
{
	const char* KEY = "default";
	auto itr = m_map2cfg.find(KEY);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	}

	int sz = SIZEOF_P3D_EMITTER_CFG + SIZEOF_P3D_SYMBOL * MAX_COMPONENTS;
	p3d_emitter_cfg* cfg_impl = (p3d_emitter_cfg*) operator new(sz);
	memset(cfg_impl, 0, sz);
	cfg_impl->syms = (p3d_symbol*)(cfg_impl+1);

	auto cfg = std::make_shared<s2::P3dEmitterCfg>(cfg_impl);
	m_map2cfg.insert(std::make_pair(KEY, cfg));
	
	return cfg;
}

s2::P3dEmitterCfgPtr PSConfigMgr::GetConfig(const std::string& filepath)
{
	auto itr = m_map2cfg.find(filepath);
	if (itr != m_map2cfg.end()) {
		return itr->second;
	} else {
		auto cfg = FileIO::LoadPSConfig(filepath);
		m_map2cfg.insert(std::make_pair(filepath, cfg));
		return cfg;
	}
}

std::string PSConfigMgr::GetFilepath(const s2::P3dEmitterCfgPtr& cfg)
{
	auto itr = m_map2cfg.begin();
	for ( ; itr != m_map2cfg.end(); ++itr) {
		if (cfg == itr->second) {
			return itr->first;
		}
	}
	return "";
}

}