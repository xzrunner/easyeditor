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
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &eanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &eanim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new sg::Frame("SgEditor", sg::FILE_TAG);
	sg::Task* task = new sg::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}