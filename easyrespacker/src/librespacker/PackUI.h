#ifndef _EASYRESPACKER_PACK_UI_H_
#define _EASYRESPACKER_PACK_UI_H_

// #include <map>
// #include <vector>
#include <string>
#include <set>

#include <json/json.h>

namespace erespacker
{

class PackUITask;

class PackUI
{
public:
	void AddTask(const std::string& filepath);
	void AddWindowTask(const std::string& filepath);
	
	void OnKnownPackID(const std::string& filepath, int id);

	void Output(const std::string& dir, Json::Value& value) const;

	void AddListener(const std::string& filepath, PackUITask* task);

	static PackUI* Instance();

private:
	PackUI();

private:
	std::map<std::string, std::set<PackUITask*> > m_task_listener;

	std::vector<PackUITask*> m_tasks;

private:
	static PackUI* m_instance;

}; // PackUI

}

#endif // _EASYRESPACKER_PACK_UI_H_