#include "main.h"
#include "Task.h"

#include <easyshadow.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyShadow", eshadow::FILE_TAG);
	eshadow::Task* task = new eshadow::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	return true;
}