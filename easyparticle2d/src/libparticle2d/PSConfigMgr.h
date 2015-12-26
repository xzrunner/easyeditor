#ifndef _EASYPARTICLE2D_PS_CONFIG_MGR_H_
#define _EASYPARTICLE2D_PS_CONFIG_MGR_H_

#include <string>
#include <map>

struct p2d_emitter_cfg;

namespace eparticle2d
{

class PSConfigMgr
{
public:
	p2d_emitter_cfg* GetDefaultConfig();

	p2d_emitter_cfg* GetConfig(const std::string& filepath);

	std::string GetFilepath(const p2d_emitter_cfg* cfg);

public:
	static PSConfigMgr* Instance();

private:
	PSConfigMgr();
	
private:
	std::map<std::string, p2d_emitter_cfg*> m_map2cfg;

	static PSConfigMgr* m_instance;

}; // PSConfigMgr

}

#endif // _EASYPARTICLE2D_PS_CONFIG_MGR_H_