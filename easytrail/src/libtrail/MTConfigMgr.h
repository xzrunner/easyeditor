#ifndef _EASYTRAIL_MT_CONFIG_MGR_H_
#define _EASYTRAIL_MT_CONFIG_MGR_H_

#include <cu/cu_macro.h>

#include <string>
#include <map>
#include <memory>

namespace s2 { class TrailEmitterCfg; }

namespace etrail
{

class MTConfigMgr
{
public:
	std::shared_ptr<s2::TrailEmitterCfg> GetDefaultConfig();

	std::shared_ptr<s2::TrailEmitterCfg> GetConfig(const std::string& filepath);

	std::string GetFilepath(const std::shared_ptr<s2::TrailEmitterCfg>& cfg);

private:
	std::map<std::string, std::shared_ptr<s2::TrailEmitterCfg>> m_map2cfg;

	CU_SINGLETON_DECLARATION(MTConfigMgr)

}; // MTConfigMgr

}

#endif // _EASYTRAIL_MT_CONFIG_MGR_H_