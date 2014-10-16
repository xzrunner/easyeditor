#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <easysketch.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(libsketch::FILE_TAG, &libsketch::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libsketch::FILE_TAG, &libsketch::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	esketch::Frame* frame = new esketch::Frame("EasySketch");
	esketch::Task* task = new esketch::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}