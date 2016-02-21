#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

#include <easysketch.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(esketch::FILE_TAG, &esketch::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(esketch::FILE_TAG, &esketch::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	esketch::Frame* frame = new esketch::Frame("EasySketch");
	esketch::Task* task = new esketch::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}