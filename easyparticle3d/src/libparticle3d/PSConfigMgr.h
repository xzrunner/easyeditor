#ifndef _EASYPARTICLE3D_PS_CONFIG_MGR_H_
#define _EASYPARTICLE3D_PS_CONFIG_MGR_H_

#include <cu/cu_macro.h>
#include <sprite2/P3dEmitterCfg.h>

#include <string>
#include <map>

namespace eparticle3d
{

class PSConfigMgr
{
public:
	s2::P3dEmitterCfgPtr GetDefaultConfig();

	s2::P3dEmitterCfgPtr GetConfig(const std::string& filepath);

	std::string GetFilepath(const s2::P3dEmitterCfgPtr& cfg);
	
private:
	std::map<std::string, s2::P3dEmitterCfgPtr> m_map2cfg;

	CU_SINGLETON_DECLARATION(PSConfigMgr)

}; // PSConfigMgr

}

#endif // _EASYPARTICLE3D_PS_CONFIG_MGR_H_