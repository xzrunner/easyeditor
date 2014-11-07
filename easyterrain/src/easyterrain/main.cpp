#include "main.h"
#include "Task.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	eterrain::Frame* frame = new eterrain::Frame("EasyTerrain");
	eterrain::Task* task = new eterrain::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}