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
	if (argc < 3) {
		std::cerr << "Params: [lua] [pngs] [dst] !" << std::endl;
		return 1;
	}

	if (argc > 1)
	{
		InitSymbolCreators();

		std::string filename = argv[1];
		libcoco::ParserLuaFile parser;
		parser.parser(filename);

		std::vector<std::string> textures;
		std::string pngs_file_head = argv[2];
		for (int i = 1; ; ++i) 
		{
			std::string filename = pngs_file_head + wxString::FromDouble(i) + ".png";
			if (wxFileName::FileExists(filename)) {
				textures.push_back(filename);
			} else {
				break;
			}
		}

		std::string outfloder = argv[3];
		parser.transToEasyFiles(textures, outfloder);
	}

	return 0;
}