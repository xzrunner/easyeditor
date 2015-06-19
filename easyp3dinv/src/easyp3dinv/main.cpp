#include "main.h"
#include "Task.h"

#include <easyp3dinv.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyP3DInv", ep3dinv::FILE_TAG);
	ep3dinv::Task* task = new ep3dinv::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}