#include "MeshFileAdapter.h"

#include <fstream>

#include "common/FileNameTools.h"
#include "common/tools.h"

namespace d2d
{

void MeshFileAdapter::load(const char* filename)
{
	std::ifstream fin(filename);

	m_imagePath = StringTools::getLine(fin);
	m_imagePath = FilenameTools::getExistFilepath(m_imagePath);

	size_t size;
	fin >> size;

	m_vertices.clear();
	m_vertices.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Vector p;
		fin >> p;
		m_vertices.push_back(p);
	}

	m_texCoords.clear();
	m_texCoords.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Vector p;
		fin >> p;
		m_texCoords.push_back(p);
	}

	fin.close();
}

} // d2d