#include "CommandLoader.h"
#include "CommandFactory.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyshape.h>
#include <easytexture.h>
#include <easyterrain2d.h>
#include <easyicon.h>
#include <easyparticle3d.h>

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);
}

static void Help()
{
	const std::vector<edb::ICommand*>& cmds 
		= edb::CommandFactory::Instance()->GetAllCmds();

	std::cout << "All commands:" << std::endl;
	for (int i = 0, n = cmds.size(); i < n; ++i) {
		edb::ICommand* cmd = cmds[i];
		std::cout << cmd->Command() << "\t" << cmd->Description() << std::endl;
	}
}

int main(int argc, char *argv[])
{
	wxInitializer initializer;
	if ( !initializer )
	{
		fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
		return -1;
	}

	edb::CommandLoader::Init();

	if (argc < 2) {
		std::cerr << "Need Command!" << std::endl;
		Help();
		return 0;
	}

	std::string scmd = argv[1];
	if (scmd == "-h" || scmd == "--help") {
		Help();
		return 0;
	}

	d2d::Config::Instance()->EnableUseDTex(false);
	d2d::Config::Instance()->EnableRender(false);

	InitSymbolCreators();

	const std::vector<edb::ICommand*>& cmds 
		= edb::CommandFactory::Instance()->GetAllCmds();
	for (int i = 0, n = cmds.size(); i < n; ++i)
	{
		edb::ICommand* cmd = cmds[i];
		if (scmd == cmd->Command()) {
			try {
				cmd->Run(argc, argv);
			} catch (d2d::Exception& e) {
				std::cout << e.what() << std::endl;
				return -1;
			}
		}
	}

	return 0;
}