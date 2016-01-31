#include "main.h"
#include "Task.h"

#include <easyshape.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyShape", libshape::FILE_TAG);
	eshape::Task* task = new eshape::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}