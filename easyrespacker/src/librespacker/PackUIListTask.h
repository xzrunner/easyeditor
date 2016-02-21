#ifndef _EASYRESPACKER_PACK_UI_LIST_TASK_H_
#define _EASYRESPACKER_PACK_UI_LIST_TASK_H_

#include "PackUITask.h"

namespace erespacker
{

class PackUIListTask : public PackUITask
{
public:
	PackUIListTask(const std::string& filepath, const Json::Value& value);
	
	virtual void OnKnownPackID(const std::string& filepath, int id);
	virtual void Output(const std::string& dir, Json::Value& value) const;

private:
 	int m_id;

	int m_cb_w, m_cb_h;

}; // PackUIListTask

}

#endif // _EASYRESPACKER_PACK_UI_LIST_TASK_H_