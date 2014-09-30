#include "main.h"
#include "Task.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	esketch::Frame* frame = new esketch::Frame("EasyImage");
	esketch::Task* task = new esketch::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}