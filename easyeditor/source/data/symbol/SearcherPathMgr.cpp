#include "SearcherPathMgr.h"
#include "FileHelper.h"
#include "StringHelper.h"
#include "std_functor.h"

#include <json/json.h>

#include <fstream>

namespace ee
{

SearcherPathMgr* SearcherPathMgr::m_instance = NULL;

static const char* FILENAME = "config.json";

SearcherPathMgr::SearcherPathMgr()
{
}

SearcherPathMgr::~SearcherPathMgr()
{
	Clear();
}

bool SearcherPathMgr::IsExist(const std::string& filepath) const
{
	bool need_search = false;

	for (int i = 0, n = m_search_path.size(); i < n; ++i) {
		const PackedRes* res = m_search_path[i];
		if (res->CanHandleFilepath(filepath)) {
			need_search = true;
			if (res->IsExist(filepath)) {
				return true;
			}
		} else {
			continue;
		}

	}

	return !need_search;
}

void SearcherPathMgr::ResetPackRes(const std::string& dirpath)
{
	Clear();

	PackedRes* res = new PackedRes(dirpath);
	m_search_path.push_back(res);
}

SearcherPathMgr* SearcherPathMgr::Instance()
{
	if (!m_instance) {
		m_instance = new SearcherPathMgr();
		m_instance->LoadConfig();
	}
	return m_instance;
}

void SearcherPathMgr::Clear()
{
	for_each(m_search_path.begin(), m_search_path.end(), 
		DeletePointerFunctor<PackedRes>());
	m_search_path.clear();
}

void SearcherPathMgr::LoadConfig()
{
	Json::Value value;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(FileHelper::GetAbsolutePath(FILENAME).c_str());
	std::locale::global(std::locale("C"));
	if (fin.fail()) {
		fin.close();
		return;
	}

	reader.parse(fin, value);
	fin.close();

	// read "tp_path"
	const char* KEY_NAME = "tp_path";
	if (value[KEY_NAME].isNull()) {
		return;
	}

	int i = 0;
	Json::Value map_val = value[KEY_NAME][i++];
	while (!map_val.isNull()) {
		std::string from = map_val["img_dir"].asString();
		std::string to = map_val["tp_output"].asString();
		PackedRes* res = new PackedRes(from, to);
		m_search_path.push_back(res);
		map_val = value[KEY_NAME][i++];
	}
}

//////////////////////////////////////////////////////////////////////////
// class SearcherPathMgr::PackedRes
//////////////////////////////////////////////////////////////////////////

SearcherPathMgr::PackedRes::
PackedRes(const std::string& res_dir,
		  const std::string& dirpath)
	: m_res_dir(res_dir)
{
	LoadCfgDir(dirpath);
	m_res_dir_absolute = FileHelper::GetAbsolutePath(m_res_dir);
}

SearcherPathMgr::PackedRes::
PackedRes(const std::string& dirpath)
{
	// from command line, no need trans
	LoadCfgDir(dirpath, false);
}

bool SearcherPathMgr::PackedRes::
IsExist(const std::string& filepath) const
{
	for (int i = 0, n = m_packed_files.size(); i < n; ++i) {
		if (filepath.find(m_packed_files[i]) != std::string::npos) {
			return true;
		}
	}
	return false;
}

bool SearcherPathMgr::PackedRes::
CanHandleFilepath(const std::string& filepath) const
{
	if (m_res_dir_absolute.empty()) {
		return true;
	} else {
		return filepath.find(m_res_dir_absolute) != std::string::npos;
	}
}

void SearcherPathMgr::PackedRes::
LoadCfgDir(const std::string& dirpath, bool need_trans)
{
	m_res_dir_absolute.clear();
	size_t i = 1;
	while (true)
	{
		std::string filepath = dirpath + StringHelper::ToString(i) + ".json";
		filepath = FileHelper::GetAbsolutePath(filepath);
		if (FileHelper::IsFileExist(filepath)) {
			LoadCfgFile(filepath);
		} else {
			break;
		}
		++i;
	}
}

void SearcherPathMgr::PackedRes::
LoadCfgFile(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int i = 0;
	Json::Value frame_val = value["frames"][i++];
	while (!frame_val.isNull()) {
		std::string filename = frame_val["filename"].asString();
		StringHelper::ToLower(filename);
		m_packed_files.push_back(filename);
		frame_val = value["frames"][i++];
	}
}

}