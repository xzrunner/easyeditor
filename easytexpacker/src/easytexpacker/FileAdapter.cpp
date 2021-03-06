#include "FileAdapter.h"

#include <ee/StringHelper.h>

#include <fstream>

namespace etexpacker
{

FileAdapter::FileAdapter()
{
	m_width = m_height = 0;
}

FileAdapter::~FileAdapter()
{
	m_width = m_height = 0;
	m_data.clear();
}

void FileAdapter::Resolve(const std::string& filepath)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	
	fin >> m_width >> m_height;

	size_t size;
	fin >> size;
	m_data.clear();
	m_data.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Item item;
		item.filepath = ee::StringHelper::GetLine(fin);
		fin >> item.left >> item.top >> item.width >> item.height;
		m_data.push_back(item);
	}

	fin.close();
}

}
