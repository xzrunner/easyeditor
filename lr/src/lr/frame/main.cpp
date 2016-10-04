#include "main.h"
#include "Frame.h"
#include "Task.h"
#include "config.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolType.h>

#include <easytexture.h>
#include <easycomplex.h>
#include <easyshape.h>
#include <easyanim.h>
#include <easyterrain2d.h>
#include <easyshadow.h>
#include <easyparticle3d.h>
#include <easyscale9.h>
#include <easyicon.h>
#include <easytext.h>
#include <easymask.h>

#include <sprite2/SymType.h>

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

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(ee::SYM_TERRAIN2D, &eterrain2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ee::SYM_TERRAIN2D, &eterrain2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTURE, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTURE, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SHAPE, &eshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SHAPE, &eshape::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ICON, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ICON, &eicon::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(ee::SYM_SHADOW, &eshadow::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ee::SYM_SHADOW, &eshadow::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTBOX, &etext::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTBOX, &etext::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MASK, &emask::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MASK, &emask::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	lr::Frame* frame = new lr::Frame("LR", lr::FILE_TAG);
	lr::Task* task = new lr::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}