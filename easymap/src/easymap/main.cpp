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
	ee::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	emap::Frame* frame = new emap::Frame("EasyMap", emap::FILE_TAG);
	emap::Task* task = new emap::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}