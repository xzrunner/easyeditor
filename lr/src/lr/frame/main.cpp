#include "main.h"
#include "Frame.h"
#include "Task.h"
#include "config.h"

#include <easytexture.h>
#include <easycomplex.h>
#include <easyshape.h>
#include <easyanim.h>
#include <easyterrain2d.h>
#include <easyshadow.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eshadow::FILE_TAG, &eshadow::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eshadow::FILE_TAG, &eshadow::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	lr::Frame* frame = new lr::Frame("LR", lr::FILE_TAG);
	lr::Task* task = new lr::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}