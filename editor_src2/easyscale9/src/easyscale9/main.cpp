#include "main.h"

#include <drag2d.h>

#include "Task.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyScale9", "scale9");
	escale9::Task* task = new escale9::Task(frame);
	frame->setTask(task);
	frame->Show(true);
	return true;
}