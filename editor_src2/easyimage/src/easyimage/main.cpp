#include "main.h"
#include "Task.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyImage", "image");
	eimage::Task* task = new eimage::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}