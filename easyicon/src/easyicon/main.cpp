#include "main.h"
#include "Task.h"

#include <easyicon.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyIcon", eicon::FILE_TAG);
	eicon::Task* task = new eicon::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}