#include "FileIO.h"
#include "Symbol.h"
#include "Shadow.h"

#include <fstream>

namespace eshadow
{

void FileIO::StoreToFile(const char* filepath, const Symbol* sym)
{
	Json::Value value;
	
	sym->GetShadow()->StoreToFile(value);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::LoadFromFile(const char* filepath, Symbol* sym)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	sym->GetShadow()->LoadFromFile(value);
}

}