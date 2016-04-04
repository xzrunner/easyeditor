#include "main.h"
#include "Task.h"
#include "config.h"

#include <ee/Frame.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyNavmesh", enav::FILE_TAG);
	enav::Task* task = new enav::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}