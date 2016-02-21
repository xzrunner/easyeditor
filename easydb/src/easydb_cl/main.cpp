#include "CommandLoader.h"
#include "CommandFactory.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Config.h>
#include <ee/Exception.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyshape.h>
#include <easytexture.h>
#include <easyterrain2d.h>
#include <easyicon.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easytext.h>

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &eanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &eanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eshape::FILE_TAG, &eshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eshape::FILE_TAG, &eshape::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etext::FILE_TAG, &etext::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etext::FILE_TAG, &etext::Sprite::Create);
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

	ee::Config::Instance()->EnableUseDTex(false);
	ee::Config::Instance()->EnableRender(false);

	InitSymbolCreators();

	const std::vector<edb::ICommand*>& cmds 
		= edb::CommandFactory::Instance()->GetAllCmds();
	for (int i = 0, n = cmds.size(); i < n; ++i)
	{
		edb::ICommand* cmd = cmds[i];
		if (scmd == cmd->Command()) {
			try {
				cmd->Run(argc, argv);
			} catch (ee::Exception& e) {
				std::cout << e.What() << std::endl;
				return -1;
			}
		}
	}

	return 0;
}