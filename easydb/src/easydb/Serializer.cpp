#include "Serializer.h"

#include <stdint.h>

namespace edb
{

void Serializer::WriteStr(std::ofstream& fout, const std::string& str)
{
	int sz = str.size();
	fout.write(reinterpret_cast<const char*>(&sz), sizeof(int16_t));
	fout.write(str.c_str(), sz);	
}

void Serializer::ReadStr(std::ifstream& fin, std::string& str)
{
	int sz = 0;
	fin.read(reinterpret_cast<char*>(&sz), sizeof(int16_t));

	static char buf[512];
	fin.read(buf, sz);
	str.assign(buf, sz);
}

void Serializer::WriteArrayInt16(std::ofstream& fout, const std::vector<int>& array)
{
	int n = array.size();
	fout.write(reinterpret_cast<const char*>(&n), sizeof(uint16_t));
	for (int i = 0; i < n; ++i) {
		fout.write(reinterpret_cast<const char*>(&array[i]), sizeof(uint16_t));
	}
}

void Serializer::ReadArrayInt16(std::ifstream& fin, std::vector<int>& array)
{
	int n = 0;
	fin.read(reinterpret_cast<char*>(&n), sizeof(uint16_t));
	array.resize(n);
	for (int i = 0; i < n; ++i) 
	{
		int id = 0;
		fin.read(reinterpret_cast<char*>(&id), sizeof(uint16_t));
		array[i] = id;
	}
}

void Serializer::WriteSetInt16(std::ofstream& fout, const std::set<int>& array)
{
	int n = array.size();
	fout.write(reinterpret_cast<const char*>(&n), sizeof(uint16_t));
	std::set<int>::const_iterator itr = array.begin();
	for ( ; itr != array.end(); ++itr) {
		fout.write(reinterpret_cast<const char*>(&(*itr)), sizeof(uint16_t));
	}
}

void Serializer::ReadSetInt16(std::ifstream& fin, std::set<int>& array)
{
	int n = 0;
	fin.read(reinterpret_cast<char*>(&n), sizeof(uint16_t));	
	for (int i = 0; i < n; ++i) 
	{
		int id = 0;
		fin.read(reinterpret_cast<char*>(&id), sizeof(uint16_t));
		array.insert(id);
	}
}

}