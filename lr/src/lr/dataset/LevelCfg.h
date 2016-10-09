#ifndef _LR_LEVEL_CFG_H_
#define _LR_LEVEL_CFG_H_

#include <vector>
#include <map>

#include <json/json.h>

namespace ee { class DynamicWidget; }

namespace lr
{

class LevelCfg
{
public:
	bool QueryLayout(const std::string& type, int& col, 
		std::vector<ee::DynamicWidget*>& widgets);

	static LevelCfg* Instance();

private:
	LevelCfg();

	void LoadFromFile(const char* filename);
	void InsertDialog(const Json::Value& val);

private:
	struct Dialog
	{
		std::string type;
		int col;
		std::vector<ee::DynamicWidget*> widgets;
	};

private:
	std::map<std::string, Dialog*> m_dialogs;
	
private:
	static LevelCfg* m_instance;

}; // LevelCfg

}

#endif // _LR_LEVEL_CFG_H_