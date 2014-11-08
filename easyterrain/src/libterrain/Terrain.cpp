#include "Terrain.h"

namespace libterrain
{

Terrain::Terrain()
	: m_hight_scale(0)
{
}

Terrain::~Terrain()
{
	delete m_data.pixels;
}

bool Terrain::LoadHeightMap(const char* filename, int size)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	std::locale::global(std::locale("C"));
	if (fin.fail()) {
		wxLogDebug("Fail to open %s", filename);
		return false;
	}

	if (m_data.pixels) {
		delete[] m_data.pixels;
	}
	int sz = size * size;
	m_data.pixels = new unsigned char[sz];
	if (m_data.pixels == NULL) {
		wxLogDebug("Could not allocate memory for%s", filename);
		return false;
	}

	memset(m_data.pixels, 0, sz);
	fin.read(reinterpret_cast<char*>(m_data.pixels), sz);

	fin.close();

	m_data.size = size;

	return true;
}

bool Terrain::SaveHeightMap(const char* filename)
{
	if (m_data.size <= 0 || m_data.pixels == NULL) {
		return false;
	}

	std::locale::global(std::locale(""));
	std::ofstream fout(filename, std::ios::binary);
	std::locale::global(std::locale("C"));
	if (fout.fail()) {
		wxLogDebug("Fail to open %s", filename);
		return false;
	}

	fout.write(reinterpret_cast<const char*>(m_data.pixels), 
		sizeof(m_data.size*m_data.size));

	fout.close();

	return true;
}

}