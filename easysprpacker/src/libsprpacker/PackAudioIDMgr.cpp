#include "PackAudioIDMgr.h"

#include <ee/FileHelper.h>

#include <json/json.h>

#include <fstream>

namespace esprpacker
{

SINGLETON_DEFINITION(PackAudioIDMgr)

PackAudioIDMgr::PackAudioIDMgr()
{
}

void PackAudioIDMgr::Init(const std::string& filepath)
{
	if (!ee::FileHelper::IsFileExist(filepath)) {
		return;
	}

	Json::Value val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, val);
	fin.close();

	for (int i = 0, n = val.size(); i < n; ++i)
	{
		std::string name = val[i]["name"].asString();
		int id = val[i]["id"].asInt();
		m_map2id.insert(std::make_pair(name, id));
	}
}

int PackAudioIDMgr::Query(const std::string& name) const
{
	std::map<std::string, int>::const_iterator itr 
		= m_map2id.find(name);
	return itr == m_map2id.end() ? -1 : itr->second;
}

}