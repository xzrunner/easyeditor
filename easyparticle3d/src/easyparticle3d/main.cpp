#include "main.h"

#include "Frame.h"
#include "Task.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easyparticle3d.h>

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	eparticle3d::Frame* frame = new eparticle3d::Frame(wxT("EasyParticle3D"), eparticle3d::FILE_TAG);
	eparticle3d::Task* task = new eparticle3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}