#include "main.h"

#include <drag2d.h>

#include "Task.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyComplex", "complex");
	ecomplex::Task* task = new ecomplex::Task(frame);
	frame->setTask(task);
	frame->Show(true);
	return true;
}