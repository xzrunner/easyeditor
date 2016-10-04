#include "main.h"
#include "Task.h"
#include "config.h"
#include "Frame.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &eanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &eanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SCALE9, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SCALE9, &escale9::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new sg::Frame("SgEditor", sg::FILE_TAG);
	sg::Task* task = new sg::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}