#include "CommandLoader.h"
#include "CommandFactory.h"

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

	InitSymbolCreators();

	const std::vector<edb::ICommand*>& cmds 
		= edb::CommandFactory::Instance()->GetAllCmds();
	for (int i = 0, n = cmds.size(); i < n; ++i)
	{
		edb::ICommand* cmd = cmds[i];
		if (scmd == cmd->Command()) {
			cmd->Run(argc, argv);
		}
	}

	return 0;
}