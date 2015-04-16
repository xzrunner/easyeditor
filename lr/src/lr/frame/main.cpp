#include "main.h"
#include "Task.h"
#include "config.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("LR", lr::FILE_TAG);
	lr::Task* task = new lr::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}