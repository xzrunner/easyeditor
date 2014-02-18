#include <easycoco.h>

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		std::string filename = argv[1];
		libcoco::ParserLuaFile parser;
		parser.parser(filename);

		std::vector<std::string> texs;
		texs.push_back("D:\\test\\characters.sc.1.pgm");
		parser.transToEasyFiles(texs, "D:\\test\\out");
	}

	return 0;
}