#include "main.h"
#include "Task.h"

#include "dtex_typedef.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("TestDtex", "");
	tdtex::Task* task = new tdtex::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}