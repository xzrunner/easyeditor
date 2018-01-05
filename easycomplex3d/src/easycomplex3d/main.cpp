#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>

#include <easy3d/Symbol.h>
#include <easy3d/Sprite.h>
#include <easy3d/ModelFile.h>

#include <easycomplex3d.h>

#include <node3/ModelType.h>

IMPLEMENT_APP(MyApp)
static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_MODEL, &e3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MODEL, &e3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	auto frame = new ee::Frame("EasyComplex3D", 
		e3d::ModelFile::Instance()->Tag(n3::MODEL_COMPLEX));
	ecomplex3d::Task* task = new ecomplex3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}