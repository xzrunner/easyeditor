#include <easycoco.h>

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		std::string filename = argv[1];
		libcoco::ParserLuaFile parser;
		parser.parser(filename);

		std::vector<std::string> texs;
// 		texs.push_back("e:\\test\\1.png");
// 		texs.push_back("e:\\test\\2.png");
// 		texs.push_back("e:\\test\\3.png");
// 		texs.push_back("e:\\test\\4.png");
		texs.push_back("e:\\test\\misc1.png");
		parser.transToEasyFiles(texs, "e:\\test\\out");
	}

	return 0;
}