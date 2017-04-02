#ifndef _EASYPARTICLE3D_PS_CONFIG_MGR_H_
#define _EASYPARTICLE3D_PS_CONFIG_MGR_H_

#include <string>
#include <map>

namespace s2 { class P3dEmitterCfg; }

namespace eparticle3d
{

class PSConfigMgr
{
public:
	s2::P3dEmitterCfg* GetDefaultConfig();

	s2::P3dEmitterCfg* GetConfig(const std::string& filepath);

	std::string GetFilepath(const s2::P3dEmitterCfg* cfg);

public:
	static PSConfigMgr* Instance();

private:
	PSConfigMgr();
	
private:
	std::map<std::string, s2::P3dEmitterCfg*> m_map2cfg;

	static PSConfigMgr* m_instance;

}; // PSConfigMgr

}

#endif // _EASYPARTICLE3D_PS_CONFIG_MGR_H_