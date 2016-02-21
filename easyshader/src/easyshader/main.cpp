#include "main.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/Frame.h>

#include <easyshader.h>
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

	ee::Frame* frame = new ee::Frame("EasyShader", eshader::FILE_TAG);
	eshader::Task* task = new eshader::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}