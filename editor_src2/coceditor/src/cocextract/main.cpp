#include <easycoco.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &anim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &anim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);
}

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		InitSymbolCreators();

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