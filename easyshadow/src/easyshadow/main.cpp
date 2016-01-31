#include "main.h"
#include "Task.h"

#include <easyshadow.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyShadow", eshadow::FILE_TAG);
	eshadow::Task* task = new eshadow::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}