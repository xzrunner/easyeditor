#ifndef _LIBRESPACKER_PACK_UI_TASK_H_
#define _LIBRESPACKER_PACK_UI_TASK_H_

#include <map>
#include <vector>
#include <string>

#include <json/json.h>

namespace librespacker
{

class PackUITask
{
public:
	void AddTask(const std::string& filepath);
	
	void OnKnownComplexID(const std::string& filepath, int id);

	void Output(const std::string& dir, Json::Value& value) const;

	static PackUITask* Instance();

private:
	PackUITask();

private:
	struct Task
	{
		std::string ui_filepath;
		std::string wrapper_filepath;
	};

	struct Result
	{
		std::string ui_filepath;
		int list_id;
	};

private:
	std::map<std::string, Task> m_tasks;

	std::vector<Result> m_results;

private:
	static PackUITask* m_instance;

}; // PackUITask

}

#endif // _LIBRESPACKER_PACK_UI_TASK_H_