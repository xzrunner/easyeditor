#ifndef _EASYRESPACKER_PACK_TAG_H_
#define _EASYRESPACKER_PACK_TAG_H_

#include <CU_Singleton.h>

#include <json/json.h>

#include <string>
#include <map>

namespace ee { class Sprite; }

namespace erespacker
{

class PackTag
{
public:
	void AddTask(const std::string& filepath, int index, const ee::Sprite* spr);

	void OnKnownPackID(const std::string& filepath, int id);

	void Output(const std::string& dir, Json::Value& value) const;

private:
	struct Task
	{
		int id;
		int index;
		std::string name;
		std::map<std::string, std::string> data;

		Task() : id(-1), index(-1) {}
	};

private:
	std::multimap<std::string, Task*> m_task_listener;

	SINGLETON_DECLARATION(PackTag);

}; // PackTag

}

#endif // _EASYRESPACKER_PACK_TAG_H_