#include "main.h"
#include "Frame.h"
#include "Task.h"
#include "config.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

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

IMPLEMENT_APP(MyApp)

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

	ee::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eshape::FILE_TAG, &eshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eshape::FILE_TAG, &eshape::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eshadow::FILE_TAG, &eshadow::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eshadow::FILE_TAG, &eshadow::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etext::FILE_TAG, &etext::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etext::FILE_TAG, &etext::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(emask::FILE_TAG, &emask::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(emask::FILE_TAG, &emask::Sprite::Create);
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