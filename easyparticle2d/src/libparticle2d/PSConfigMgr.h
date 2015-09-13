#ifndef _EASYPARTICLE2D_PS_CONFIG_MGR_H_
#define _EASYPARTICLE2D_PS_CONFIG_MGR_H_

#include <string>
#include <map>

struct p2d_ps_config;

namespace eparticle2d
{

class PSConfigMgr
{
public:
	p2d_ps_config* GetDefaultConfig();

	p2d_ps_config* GetConfig(const std::string& filepath);

	std::string GetFilepath(const p2d_ps_config* cfg);

public:
	static PSConfigMgr* Instance();

private:
	PSConfigMgr();
	
private:
	std::map<std::string, p2d_ps_config*> m_map2cfg;

	static PSConfigMgr* m_instance;

}; // PSConfigMgr

}

#endif // _EASYPARTICLE2D_PS_CONFIG_MGR_H_