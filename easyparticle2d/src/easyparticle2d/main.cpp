#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easyparticle2d.h>

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &eanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &eanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyParticle2D", eparticle2d::FILE_TAG);
	eparticle2d::Task* task = new eparticle2d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}