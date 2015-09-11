#ifndef _LR_UNIT_CFG_H_
#define _LR_UNIT_CFG_H_

#include <vector>
#include <string>
#include <json/json.h>

namespace lr
{

class UnitCfg
{
public:
	static UnitCfg* Instance();

public:
	struct Item
	{
		std::string key;
		std::string title;
	};

	std::vector<Item> type_list, camp_list, squad_list, dir_list;

private:
	UnitCfg() {}

	void LoadFromFile(const char* filename);

	static void LoadList(const Json::Value& val, const std::string& type, 
		std::vector<Item>& list);

private:
	static bool m_loaded;

	static UnitCfg* m_instance;

}; // UnitCfg

}

#endif // _LR_UNIT_CFG_H_