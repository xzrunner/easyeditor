#ifndef _EASYPARTICLE3D_PS_CONFIG_MGR_H_
#define _EASYPARTICLE3D_PS_CONFIG_MGR_H_

#include <string>
#include <map>

struct p3d_emitter_cfg;

namespace eparticle3d
{

class PSConfigMgr
{
public:
	p3d_emitter_cfg* GetDefaultConfig();

	p3d_emitter_cfg* GetConfig(const std::string& filepath);

	std::string GetFilepath(const p3d_emitter_cfg* cfg);

public:
	static PSConfigMgr* Instance();

private:
	PSConfigMgr();
	
private:
	std::map<std::string, p3d_emitter_cfg*> m_map2cfg;

	static PSConfigMgr* m_instance;

}; // PSConfigMgr

}

#endif // _EASYPARTICLE3D_PS_CONFIG_MGR_H_