#include "PackTag.h"

#include <ee/Sprite.h>
#include <ee/StringHelper.h>
#include <ee/FileHelper.h>

#include <assert.h>

namespace erespacker
{

SINGLETON_DEFINITION(PackTag);

PackTag::PackTag() 
{

}

void PackTag::AddTask(const std::string& filepath, int index, const ee::Sprite* spr)
{
	const std::string& tag = spr->GetTag();
	if (tag.empty() || tag.find('=') == std::string::npos) {
		return;
	}

	std::multimap<std::string, Task*>::iterator itr = m_task_listener.find(filepath);

	Task* task = new Task;

	task->index = index;

	std::string name;
	s2::SprNameMap::Instance()->IDToStr(spr->GetName(), name);
	if (!name.empty() && name[0] != '_') {
		task->name = name;
	}
	
	std::vector<std::string> tags;
	ee::StringHelper::Split(spr->GetTag(), ";", tags);
	for (int i = 0, n = tags.size(); i < n; ++i) 
	{
		int pos = tags[i].find("=");
		if (pos == std::string::npos) {
			continue;
		}
		std::string key = tags[i].substr(0, pos);
		std::string val = tags[i].substr(pos + 1);
		task->data.insert(std::make_pair(key, val));
	}

	m_task_listener.insert(std::make_pair(filepath, task));
}

void PackTag::OnKnownPackID(const std::string& filepath, int id)
{
	std::string key = ee::FileHelper::FormatFilepathAbsolute(filepath);
	std::multimap<std::string, Task*>::const_iterator 
		itr_begin = m_task_listener.lower_bound(key),
		itr_end = m_task_listener.upper_bound(key);
	std::multimap<std::string, Task*>::const_iterator itr = itr_begin;
	for ( ; itr != itr_end; ++itr) {
		itr->second->id = id;
	}
}

void PackTag::Output(const std::string& dir, Json::Value& value) const
{
	std::multimap<std::string, Task*>::const_iterator itr 
		= m_task_listener.begin();
	for ( ; itr != m_task_listener.end(); ++itr) 
	{
		Task* task = itr->second;
		if (task->id == -1) {
			continue;
		}

		Json::Value val;

		val["id"] = task->id;
		val["index"] = task->index;
		if (!task->name.empty()) {
			val["name"] = task->name;
		}

		std::map<std::string, std::string>::iterator itr_data 
			= task->data.begin();
		for ( ; itr_data != task->data.end(); ++itr_data) {
			val[itr_data->first] = itr_data->second;
		}

		value[value.size()] = val;
	}
}

}