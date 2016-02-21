#include "main.h"
#include "Task.h"

#include <ee/Frame.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("TestDtex", "");
	tdtex::Task* task = new tdtex::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}