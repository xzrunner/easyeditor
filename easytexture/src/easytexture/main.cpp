#include "main.h"
#include "Task.h"

#include <easytexture.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyTexture", etexture::FILE_TAG);
	etexture::Task* task = new etexture::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}