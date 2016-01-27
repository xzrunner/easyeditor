#include "main.h"
#include "Task.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	eimage::Frame* frame = new eimage::Frame("EasyImage");
	eimage::Task* task = new eimage::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}