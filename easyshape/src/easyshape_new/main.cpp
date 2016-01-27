#include "main.h"
#include "Task.h"

#include <easyshape.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyanim.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new d2d::Frame("EasyShape", libshape::FILE_TAG);
	eshape::Task* task = new eshape::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		d2d::Config::Instance()->EnableUseDTex(false);
		d2d::Config::Instance()->EnableRender(false);
		wxString path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
		d2d::Config::Instance()->EnableUseDTex(true);
		d2d::Config::Instance()->EnableRender(true);
	}

	return true;
}