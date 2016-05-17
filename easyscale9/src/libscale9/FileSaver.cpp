#include "FileSaver.h"
#include "Symbol.h"

#include <fstream>

namespace escale9
{

void FileSaver::Store(const char* filepath, const Symbol& symbol)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	sm::vec2 sz = symbol.GetSize().Size();
	value["width"] = sz.x;
	value["height"] = sz.y;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

} // escale9