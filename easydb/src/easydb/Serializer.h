#ifndef _EASYDB_SERIALIZER_H_
#define _EASYDB_SERIALIZER_H_

#include <fstream>
#include <vector>

namespace edb
{

class Serializer
{
public:
	static void WriteStr(std::ofstream& fout, const std::string& str);
	static void ReadStr(std::ifstream& fin, std::string& str);

	static void WriteArrayInt16(std::ofstream& fout, const std::vector<int>& array);
	static void ReadArrayInt16(std::ifstream& fin, std::vector<int>& array);

}; // Serializer

}

#endif // _EASYDB_SERIALIZER_H_