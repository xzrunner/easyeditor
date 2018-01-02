#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

#include <easysketch.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex3d::FILE_TAG, &ecomplex3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex3d::FILE_TAG, &ecomplex3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ecomplex3d::Frame* frame = new ecomplex3d::Frame("EasySketch");
	ecomplex3d::Task* task = new ecomplex3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}