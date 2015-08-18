#ifndef _EASYPARTICLE3D_PS_CONFIG_MGR_H_
#define _EASYPARTICLE3D_PS_CONFIG_MGR_H_

#include <string>
#include <map>

struct ps_cfg_3d;

namespace eparticle3d
{

class PSConfigMgr
{
public:
	ps_cfg_3d* GetDefaultConfig();

	ps_cfg_3d* GetConfig(const std::string& filepath);

	std::string GetFilepath(const ps_cfg_3d* cfg);

public:
	static PSConfigMgr* Instance();

private:
	PSConfigMgr();
	
private:
	std::map<std::string, ps_cfg_3d*> m_map2cfg;

	static PSConfigMgr* m_instance;

}; // PSConfigMgr

}

#endif // _EASYPARTICLE3D_PS_CONFIG_MGR_H_