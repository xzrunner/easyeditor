#include "main.h"
#include "Task.h"

#include <easyicon.h>

#include <ee/Frame.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyIcon", eicon::FILE_TAG);
	eicon::Task* task = new eicon::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}