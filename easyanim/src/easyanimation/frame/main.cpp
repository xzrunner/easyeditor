#include "main.h"

#include "Frame.h"
#include "Task.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyparticle3d.h>
#include <easyicon.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	eanim::Frame* frame = new eanim::Frame("easyanim", libanim::FILE_TAG);
	eanim::Task* task = new eanim::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		task->GetEditPanel()->GetCanvas()->SetCurrentCanvas();

		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}