#include "main.h"
#include "Task.h"

#include <ee/SymbolFile.h>
#include <ee/Frame.h>

#include <sprite2/SymType.h>

#include <easyicon.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyIcon", ee::SymbolFile::Instance()->Tag(s2::SYM_ICON));
	eicon::Task* task = new eicon::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}