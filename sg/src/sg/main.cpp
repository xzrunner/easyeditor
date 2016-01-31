#include "main.h"
#include "Task.h"
#include "config.h"
#include "Frame.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);
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