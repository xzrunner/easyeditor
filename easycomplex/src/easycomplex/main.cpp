#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easyejoy2d.h>
#include <easyterrain2d.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &anim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &anim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eejoy2d::FILE_TAG, &eejoy2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eejoy2d::FILE_TAG, &eejoy2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);
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