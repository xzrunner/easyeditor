#include "FileSaver.h"
#include "Symbol.h"

namespace escale9
{

void FileSaver::store(const char* filepath, const Symbol& symbol)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filepath);
	reader.parse(fin, value);
	fin.close();

	value["width"] = symbol.getSize().xLength();
	value["height"] = symbol.getSize().yLength();

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath);
	writer.write(fout, value);
	fout.close();
}

} // escale9