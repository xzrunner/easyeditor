#include "main.h"
#include "Task.h"

#include <ee/Frame.h>

#include <easytemplate.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyTemplate", etemplate::FILE_TAG);
	etemplate::Task* task = new etemplate::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}