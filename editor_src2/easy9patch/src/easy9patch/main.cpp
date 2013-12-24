#include "main.h"

#include <drag2d.h>

#include "Task.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("Easy9Patch", "9patch");
	e9patch::Task* task = new e9patch::Task(frame);
	frame->setTask(task);
	frame->Show(true);
	return true;
}