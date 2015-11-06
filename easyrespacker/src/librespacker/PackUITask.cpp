#include "PackUITask.h"

#include <drag2d.h>

namespace librespacker
{

PackUITask* PackUITask::m_instance = NULL;

static const std::string UI_WINDOW	= "window";
static const std::string UI_LIST	= "list";

PackUITask::PackUITask()
{
}

void PackUITask::AddTask(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string type = value["type"].asString();
	if (type != UI_LIST) {
		return;
	}

	Task task;
	task.ui_filepath = filepath;
	task.wrapper_filepath = value["wrapper filepath"].asString();
	task.wrapper_filepath = d2d::FilenameTools::getAbsolutePathFromFile(filepath, task.wrapper_filepath);
	task.wrapper_filepath = d2d::FilenameTools::FormatFilepathAbsolute(task.wrapper_filepath);
	m_tasks.insert(std::make_pair(task.wrapper_filepath, task));
}

void PackUITask::OnKnownComplexID(const std::string& filepath, int id)
{
	std::map<std::string, Task>::iterator itr = m_tasks.find(filepath);
	if (itr == m_tasks.end()) {
		return;
	}

	Result ret;
	ret.ui_filepath = itr->second.ui_filepath;
	ret.list_id = id;
	m_results.push_back(ret);
}

void PackUITask::Output(const std::string& dir, Json::Value& value) const
{
	for (int i = 0, n = m_results.size(); i < n; ++i) {
		const Result& r = m_results[i];
		Json::Value item_val;
		item_val["filepath"] = d2d::FilenameTools::getRelativePath(dir, r.ui_filepath).ToStdString();
		item_val["wrapper id"] = r.list_id;
		item_val["type"] = UI_LIST;
		value[value.size()] = item_val;
	}
}

PackUITask* PackUITask::Instance()
{
	if (!m_instance) {
		m_instance = new PackUITask;
	}
	return m_instance;
}

}