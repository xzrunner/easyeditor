#include "main.h"
#include "Task.h"

#include <easyscale9.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyScale9", escale9::FILE_TAG);
	escale9::Task* task = new escale9::Task(frame);
	frame->setTask(task);
	frame->Show(true);
	return true;
}