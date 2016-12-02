#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>
#include <ee/Config.h>
#include <ee/SymbolFile.h>

#include <easyshape.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyanim.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTURE, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTURE, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &libanim::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyShape", ee::SymbolFile::Instance()->Tag(s2::SYM_SHAPE));
	eshape::Task* task = new eshape::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		ee::Config::Instance()->EnableUseDTex(false);
		ee::Config::Instance()->EnableRender(false);
		std::string path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
		ee::Config::Instance()->EnableUseDTex(true);
		ee::Config::Instance()->EnableRender(true);
	}

	return true;
}