#include "main.h"

#include <drag2d.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

#include "Task.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator("complex", &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator("complex", &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator("anim", &anim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator("anim", &anim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator("scale9", &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator("scale9", &escale9::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new ecomplex::Frame("EasyComplex", "complex");
	ecomplex::Task* task = new ecomplex::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		ecomplex::StagePanel* stage = task->getStagePanel();
		stage->getCanvas()->SetCurrentCanvas();

		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}