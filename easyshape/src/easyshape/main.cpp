#include "main.h"
#include "Task.h"

#include <easyshape.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyanim.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyShape", eshape::FILE_TAG);
	eshape::Task* task = new eshape::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		ee::Config::Instance()->EnableUseDTex(false);
		ee::Config::Instance()->EnableRender(false);
		wxString path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
		ee::Config::Instance()->EnableUseDTex(true);
		ee::Config::Instance()->EnableRender(true);
	}

	return true;
}