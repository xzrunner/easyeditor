#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>

#include <easycomplex3d.h>

IMPLEMENT_APP(MyApp)
static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_MODEL, &ecomplex3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MODEL, &ecomplex3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	auto frame = new ee::Frame("EasyComplex3D", ecomplex3d::FILE_TAG);
	ecomplex3d::Task* task = new ecomplex3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}