#include "main.h"
#include "Task.h"

#include <easytemplate.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyTemplate", etemplate::FILE_TAG);
	etemplate::Task* task = new etemplate::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}