#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/FileType.h>

#include <easytrail.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyTrail", ee::FileType::GetTag(ee::FileType::e_trail));
	etrail::Task* task = new etrail::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}