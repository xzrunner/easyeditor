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
	str_replace(path_fixed, "/", "\\");

	std::map<std::string, int>::const_iterator itr = m_ids.find(path_fixed);
	if (itr == m_ids.end()) {
		std::map<std::string, int>::const_iterator itr = m_ids.begin();
		for ( ; itr != m_ids.end(); ++itr) {
			if (itr->first.find(path_fixed) != std::string::npos) {
				return itr->second;
			}
		}
		throw Exception("Cannot find image %s\n", path);
	}
	return itr->second;
}

void ImageIDer::Load(const std::string& filepath)
{
	std::string dir = get_file_dir(filepath);

	std::ifstream fin(filepath.c_str());
	std::string line;
	int id = 1;
	while (std::getline(fin, line)) {
		std::string path = dir + "\\" + line;
		std::string key = path.substr(0, path.find_last_of('.'));

//		str_replace(key, "\\", "/");
		str_replace(key, "/", "\\");

		m_ids.insert(std::make_pair(key, id++));
	}
	fin.close();
}

}