#include "main.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

#include "Task.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &anim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &anim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new ecomplex::Frame("EasyComplex", ecomplex::FILE_TAG);
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