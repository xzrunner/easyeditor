#include "CombinationFileAdapter.h"
#include "FileNameTools.h"

#include <fstream>

#include "common/tools.h"

namespace d2d
{

void CombinationFileAdapter::load(const char* filename)
{
	m_data.clear();

	std::ifstream fin(filename);

	wxString dir = FilenameTools::getFileDir(filename) + "\\";

	size_t size;
	fin >> size;
	m_data.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Entry entry;

		entry.filepath = StringTools::getLine(fin);
		entry.filepath = FilenameTools::getExistFilepath(entry.filepath, dir);

		std::string strLine = StringTools::getLine(fin);
		std::stringstream ss(strLine);
		ss >> entry.pos >> entry.angle >> entry.scale >> entry.xMirror >> entry.yMirror;

		m_data.push_back(entry);
	}

	fin.close();
}

} // d2d