#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <easymap.h>
#include <easytexture.h>
#include <easycomplex.h>
#include <easyshape.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	emap::Frame* frame = new emap::Frame("EasyMap", emap::FILE_TAG);
	emap::Task* task = new emap::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}