#include "main.h"
#include "Task.h"

#include <easyp3dinv.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyP3DInv", ep3dinv::FILE_TAG);
	ep3dinv::Task* task = new ep3dinv::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}