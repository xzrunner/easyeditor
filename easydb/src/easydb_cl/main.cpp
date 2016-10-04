#include "CommandLoader.h"
#include "CommandFactory.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Config.h>
#include <ee/Exception.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

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
#include <easymask.h>
#include <easytrail.h>

#include <sprite2/SymType.h>

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &eanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &eanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SCALE9, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SCALE9, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MESH, &emesh::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MESH, &emesh::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SHAPE, &eshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SHAPE, &eshape::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTURE, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTURE, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(ee::SYM_TERRAIN2D, &eterrain2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ee::SYM_TERRAIN2D, &eterrain2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ICON, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ICON, &eicon::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTBOX, &etext::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTBOX, &etext::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MASK, &emask::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MASK, &emask::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TRAIL, &etrail::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TRAIL, &etrail::Sprite::Create);
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