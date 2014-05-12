#include "CircleFileAdapter.h"

#include <fstream>

#include "common/tools.h"

namespace d2d
{

void CircleFileAdapter::load(const char* filename)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));

	m_imgPath = StringTools::getLine(fin);
	fin >> m_width >> m_height;

	fin.close();
}

} // d2d