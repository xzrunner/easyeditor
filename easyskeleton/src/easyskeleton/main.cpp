#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasySkeleton", ee::SymbolFile::Instance()->Tag(s2::SYM_SKELETON));
	eskeleton::Task* task = new eskeleton::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}