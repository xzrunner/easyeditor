#include "main.h"
#include "Task.h"

#include <ee/Frame.h>

#include <easymask.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyMask", emask::FILE_TAG);
	emask::Task* task = new emask::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}