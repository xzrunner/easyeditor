#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/FileType.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasySkeleton", ee::FileType::GetTag(ee::FILE_SKELETON));
	eskeleton::Task* task = new eskeleton::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}