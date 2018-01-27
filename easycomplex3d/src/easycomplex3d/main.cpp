#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>

#include <ee3/NodeSymbol.h>
#include <ee3/NodeSprite.h>

#include <easycomplex3d.h>

IMPLEMENT_APP(MyApp)
static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_MODEL, &ee3::NodeSymbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MODEL, &ee3::NodeSprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	auto frame = new ee::Frame("EasyComplex3D", "complex3d");
	ecomplex3d::Task* task = new ecomplex3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}