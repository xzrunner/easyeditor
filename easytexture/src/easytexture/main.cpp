#include "main.h"
#include "Task.h"

#include <easytexture.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyTexture", etexture::FILE_TAG);
	etexture::Task* task = new etexture::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}