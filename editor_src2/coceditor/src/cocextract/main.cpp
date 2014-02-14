#include "ParserLuaFile.h"

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		std::string filename = argv[1];
		cocextract::ParserLuaFile parser;
		parser.parser(filename);
	}	

	return 0;
}