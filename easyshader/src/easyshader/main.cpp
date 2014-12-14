#include "main.h"
#include "Task.h"

#include <easyshader.h>
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

	d2d::Frame* frame = new d2d::Frame("EasyShader", eshader::FILE_TAG);
	eshader::Task* task = new eshader::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}