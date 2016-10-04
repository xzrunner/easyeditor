#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolType.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easyejoy2d.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyshape.h>
#include <easyicon.h>
#include <easyshadow.h>
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

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(ee::SYM_EJOY2D, &eejoy2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ee::SYM_EJOY2D, &eejoy2d::Sprite::Create);

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

	ee::Frame* frame = new ee::Frame("EasyMask", ee::SymbolFile::Instance()->Tag(s2::SYM_MASK));
	emask::Task* task = new emask::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}