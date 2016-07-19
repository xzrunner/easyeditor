#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/FileType.h>
#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

#include <easytrail.h>
#include <easyanim.h>
#include <easycomplex.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eanim::FILE_TAG, &eanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eanim::FILE_TAG, &eanim::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ee::Frame("EasyTrail", ee::FileType::GetTag(ee::FileType::e_trail));
	etrail::Task* task = new etrail::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}