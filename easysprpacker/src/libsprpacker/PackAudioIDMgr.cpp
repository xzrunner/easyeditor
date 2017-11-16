#include "PackAudioIDMgr.h"

#include <ee/FileHelper.h>

#include <json/json.h>

#include <fstream>

namespace esprpacker
{

CU_SINGLETON_DEFINITION(PackAudioIDMgr)

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

int PackAudioIDMgr::Query(const std::string& filepath) const
{
	for (auto itr = m_map2id.begin(); itr != m_map2id.end(); ++itr) {
		if (filepath.find_last_of(itr->first) != std::string::npos) {
			return itr->second;
		}
	}
	return -1;
}

}