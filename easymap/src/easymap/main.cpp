#include "main.h"
#include "Task.h"

#include <easymap.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyMap", emap::FILE_TAG);
	emap::Task* task = new emap::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}