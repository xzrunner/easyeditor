#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/FileType.h>

#include <easybone.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyBone", ee::FileType::GetTag(ee::FILE_BONE));
	ebone::Task* task = new ebone::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}