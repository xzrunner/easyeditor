#ifndef _EASYUI_TID_CFG_H_
#define _EASYUI_TID_CFG_H_

#include <drag2d.h>

namespace eui
{

class TidCfg
{
public:
	static TidCfg* Instance();

	void LoadConfig();

	std::string Query(const std::string& key) const;

private:
	TidCfg() {}
	
private:
	std::map<std::string, std::string> m_map_tid;

private:
	static TidCfg* m_instance;

}; // TidCfg

}

#endif // _EASYUI_TID_CFG_H_