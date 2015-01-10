#include "main.h"
#include "Task.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	eterrain3d::Frame* frame = new eterrain3d::Frame("EasyTerrain");
	eterrain3d::Task* task = new eterrain3d::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}