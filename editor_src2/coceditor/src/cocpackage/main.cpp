#include "../coceditor/Context.h"
#include "../coceditor/COCCode.h"

#include <iostream>

#include <easybuilder.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

#define CHARACTER

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
	if (argc < 4) {
		std::cerr << "Need Resource Dir & Texture Packer Filepath & Output filepath !" << std::endl;
		return 1;
	}

	InitSymbolCreators();

	if (argc > 4)
	{
		double scale;
		wxString(argv[4]).ToDouble(&scale);
		coceditor::Context::Instance()->scale = scale;
	}

	std::string dirpath = argv[1];
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dirpath, files);

	try {
		for (size_t i = 0, n = files.size(); i < n; ++i)
		{
			wxFileName filename(files[i]);
			filename.Normalize();
			wxString filepath = filename.GetFullPath();
			
			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)
				|| d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
			{
				// todo release symbol
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
				coceditor::Context::Instance()->symbols.push_back(symbol);
			}
		}
	} catch (d2d::Exception& e) {
		std::cerr << e.what() << std::endl;
	}

	std::string texpackerpath = argv[2];
	size_t i = 1;
	while (true)
	{
		std::string path = texpackerpath + wxString::FromDouble(i) + ".json";
		if (wxFileName::FileExists(path))
			coceditor::Context::Instance()->texMgr.initTexture(path, i-1);
		else
			break;
		++i;
	}

	ebuilder::CodeGenerator gen;
	coceditor::COCCode code(gen);
	code.resolve();
	std::ofstream fout(argv[3]);
	fout << gen.toText() << std::endl;
	fout.close();

	return 0;
}