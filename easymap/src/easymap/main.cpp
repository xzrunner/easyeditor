#include "main.h"
#include "Task.h"

#include <easymap.h>
#include <easytexture.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new d2d::Frame("EasyMap", emap::FILE_TAG);
	emap::Task* task = new emap::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}