#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

#include <easytrail.h>
#include <easyanim.h>
#include <easycomplex.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &libanim::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyTrail", ee::SymbolFile::Instance()->Tag(s2::SYM_TRAIL));
	etrail::Task* task = new etrail::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}