#ifndef _LIBRESPACKER_PACK_UI_TASK_H_
#define _LIBRESPACKER_PACK_UI_TASK_H_

#include <string>
#include <json/json.h>

namespace librespacker
{

class PackUITask
{
public:
	PackUITask(const std::string& filepath) : m_filepath(filepath) {}
	virtual ~PackUITask() {}
	virtual void OnKnownPackID(const std::string& filepath, int id) = 0;
	virtual void Output(const std::string& dir, Json::Value& value) const = 0;

protected:
	std::string m_filepath;

}; // PackUITask

}

#endif // _LIBRESPACKER_PACK_UI_TASK_H_