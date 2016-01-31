#include "main.h"
#include "Task.h"

#include <easyshader.h>
#include <easysketch.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(libsketch::FILE_TAG, &libsketch::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libsketch::FILE_TAG, &libsketch::Sprite::Create);
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