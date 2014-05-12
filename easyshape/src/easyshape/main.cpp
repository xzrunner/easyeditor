#include "main.h"
#include "Task.h"

#include <easyshape.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new d2d::Frame("EasyShape", libshape::FILE_TAG);
	eshape::Task* task = new eshape::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}