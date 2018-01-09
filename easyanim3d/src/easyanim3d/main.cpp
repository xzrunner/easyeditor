#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>

#include <easynode3d/Symbol.h>
#include <easynode3d/Sprite.h>
#include <easynode3d/ModelFile.h>

#include <easyanim3d.h>

#include <node3/ModelType.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_MODEL, &enode3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MODEL, &enode3d::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyAnim3D", 
		enode3d::ModelFile::Instance()->Tag(n3::MODEL_ANIMATION));
	eanim3d::Task* task = new eanim3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}