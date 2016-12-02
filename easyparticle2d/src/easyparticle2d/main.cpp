#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>
#include <ee/SymbolFile.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easyparticle2d.h>

#include <sprite2/SymType.h>

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &libanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SCALE9, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SCALE9, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyParticle2D", ee::SymbolFile::Instance()->Tag(s2::SYM_PARTICLE2D));
	eparticle2d::Task* task = new eparticle2d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}