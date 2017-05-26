#ifndef _EASYTRAIL_MT_CONFIG_MGR_H_
#define _EASYTRAIL_MT_CONFIG_MGR_H_

#include <string>
#include <map>

namespace s2 { class TrailEmitterCfg; }

namespace etrail
{

class MTConfigMgr
{
public:
	s2::TrailEmitterCfg* GetDefaultConfig();

	s2::TrailEmitterCfg* GetConfig(const std::string& filepath);

	std::string GetFilepath(const s2::TrailEmitterCfg* cfg);

public:
	static MTConfigMgr* Instance();

private:
	MTConfigMgr();

private:
	std::map<std::string, s2::TrailEmitterCfg*> m_map2cfg;

	static MTConfigMgr* m_instance;

}; // MTConfigMgr

}

#endif // _EASYTRAIL_MT_CONFIG_MGR_H_