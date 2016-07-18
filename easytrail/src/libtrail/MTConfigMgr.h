#ifndef _EASYTRAIL_MT_CONFIG_MGR_H_
#define _EASYTRAIL_MT_CONFIG_MGR_H_

#include <string>
#include <map>

struct t2d_emitter_cfg;

namespace etrail
{

class MTConfigMgr
{
public:
	t2d_emitter_cfg* GetDefaultConfig();

	t2d_emitter_cfg* GetConfig(const std::string& filepath);

	std::string GetFilepath(const t2d_emitter_cfg* cfg);

public:
	static MTConfigMgr* Instance();

private:
	MTConfigMgr();

private:
	std::map<std::string, t2d_emitter_cfg*> m_map2cfg;

	static MTConfigMgr* m_instance;

}; // MTConfigMgr

}

#endif // _EASYTRAIL_MT_CONFIG_MGR_H_