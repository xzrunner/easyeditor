#include "main.h"
#include "Task.h"

#include <easyscale9.h>

#include <ee/Frame.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyScale9", escale9::FILE_TAG);
	escale9::Task* task = new escale9::Task(frame);
	frame->SetTask(task);
	frame->Show(true);
	return true;
}