#include "ImageIDer.h"
#include "Exception.h"
#include "tools.h"

#include <fstream>

namespace epbin
{

ImageIDer::ImageIDer(const std::string& filepath)
{
	Load(filepath);
}

int ImageIDer::Query(const std::string& path) const
{
	std::string path_fixed = path.substr(0, path.find_last_of('.'));
	str_replace(path_fixed, "\\", "/");
	std::map<std::string, int>::const_iterator itr = m_ids.find(path_fixed);
	if (itr == m_ids.end()) {
		throw Exception("Cannot find image %s\n", path);
	}
	return itr->second;
}

void ImageIDer::Load(const std::string& filepath)
{
	std::ifstream fin(filepath.c_str());
	std::string line;
	int id = 1;
	while (std::getline(fin, line)) {
		std::string key = line.substr(0, line.find_last_of('.'));

		str_replace(key, "\\", "/");
//		str_replace(key, "/", "\\");

		m_ids.insert(std::make_pair(key, id++));
	}
	fin.close();
}

}