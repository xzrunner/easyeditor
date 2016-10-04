#include "main.h"
#include "Frame.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>

IMPLEMENT_APP(MyApp)

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
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	edb::Frame* frame = new edb::Frame("EasyDB");
	frame->Show(true);

	return true;
}