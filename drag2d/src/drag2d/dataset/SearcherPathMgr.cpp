#include "SearcherPathMgr.h"

#include "common/tools.h"
#include "common/FileNameTools.h"

#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <JSON/json.h>

namespace d2d
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

bool SearcherPathMgr::IsExist(const wxString& filepath) const
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
 	wxFileName filename(FILENAME);
#ifndef _DEBUG
	wxStandardPathsBase& stdp = wxStandardPaths::Get();
	wxString exe_path = stdp.GetExecutablePath();
	filename.MakeAbsolute(FilenameTools::getFileDir(exe_path));
#endif
 	filename.Normalize();

	Json::Value value;

	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename.GetFullPath().fn_str());
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

	wxFileName filename(m_res_dir);
#ifndef _DEBUG
	wxStandardPathsBase& stdp = wxStandardPaths::Get();
	wxString exe_path = stdp.GetExecutablePath();
	filename.MakeAbsolute(FilenameTools::getFileDir(exe_path));
#endif
	filename.Normalize();
	m_res_dir_absolute = filename.GetFullPath().Lower();
}

SearcherPathMgr::PackedRes::
PackedRes(const std::string& dirpath)
{
	// from command line, no need trans
	LoadCfgDir(dirpath, false);
}

bool SearcherPathMgr::PackedRes::
IsExist(const wxString& filepath) const
{
	for (int i = 0, n = m_packed_files.size(); i < n; ++i) {
		if (filepath.Contains(m_packed_files[i])) {
			return true;
		}
	}

	return false;
}

bool SearcherPathMgr::PackedRes::
CanHandleFilepath(const wxString& filepath) const
{
	if (m_res_dir_absolute.IsEmpty()) {
		return true;
	} else {
		return filepath.Contains(m_res_dir_absolute);
	}
}

void SearcherPathMgr::PackedRes::
LoadCfgDir(const std::string& dirpath, bool need_trans)
{
	m_res_dir_absolute = wxEmptyString;

#ifndef _DEBUG
	wxStandardPathsBase& stdp = wxStandardPaths::Get();
	wxString exe_path = stdp.GetExecutablePath();
#endif

	size_t i = 1;
	while (true)
	{
		std::string path = dirpath + wxString::FromDouble(i) + ".json";

		wxFileName filename(path);
#ifndef _DEBUG
		if (need_trans) {
			filename.MakeAbsolute(FilenameTools::getFileDir(exe_path));
		}
#endif
		filename.Normalize();
		path = filename.GetFullPath().Lower();
		if (wxFileName::FileExists(path)) {
			LoadCfgFile(path);
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
		StringTools::toLower(filename);
		m_packed_files.push_back(filename);
		frame_val = value["frames"][i++];
	}
}

}