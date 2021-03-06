#include "PackUI.h"
#include "pack_ui_cfg.h"

#include "PackUIListTask.h"
#include "PackUIWindowTask.h"
#include "PackUIWrapperTask.h"

#include <ee/FileHelper.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <fstream>

namespace erespacker
{

PackUI* PackUI::m_instance = NULL;

PackUI::PackUI()
{
}

std::string PackUI::AddTask(const std::string& filepath)
{
	std::string ret;

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string type = value["type"].asString();
	PackUITask* task = NULL;
	if (type == UI_LIST) {
		task = new PackUIListTask(filepath, value);
		ret = value["wrapper filepath"].asString();
	} else if (type == UI_WRAPPER) {
		task = new PackUIWrapperTask(filepath, value);
		ret = value["wrapper filepath"].asString();
	} else {
//		throw ee::Exception("PackUI unknown task %s", type);
		return ret;
	}
	m_tasks.push_back(task);

	return ret;
}

void PackUI::AddWindowTask(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	assert(ee::SymbolFile::Instance()->Type(filepath) == ee::SYM_UIWND);
	m_tasks.push_back(new PackUIWindowTask(filepath, value));
}

void PackUI::OnKnownPackID(const std::string& filepath, int id)
{
	std::string key = ee::FileHelper::FormatFilepathAbsolute(filepath);

	std::map<std::string, std::set<PackUITask*> >::iterator 
		itr = m_task_listener.find(key);
	if (itr == m_task_listener.end()) {
		return;
	}

	std::set<PackUITask*>::iterator itr_task = itr->second.begin();
	for ( ; itr_task != itr->second.end(); ++itr_task) {
		(*itr_task)->OnKnownPackID(key, id);
	}
}

void PackUI::Output(const std::string& dir, Json::Value& value) const
{
	for (int i = 0, n = m_tasks.size(); i < n; ++i) {
		m_tasks[i]->Output(dir, value);
	}
}

void PackUI::AddListener(const std::string& filepath, PackUITask* task)
{
	std::map<std::string, std::set<PackUITask*> >::iterator 
		itr = m_task_listener.find(filepath);
	if (itr == m_task_listener.end()) {
		std::set<PackUITask*> tasks;
		tasks.insert(task);
		m_task_listener.insert(std::make_pair(filepath, tasks));
	} else {
		itr->second.insert(task);
	}
}

PackUI* PackUI::Instance()
{
	if (!m_instance) {
		m_instance = new PackUI;
	}
	return m_instance;
}

}