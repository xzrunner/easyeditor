#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/SymbolFile.h>

#include <easyp3dinv.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyP3DInv", ee::SymbolFile::Instance()->Tag(ee::SYM_P3DINV));
	ep3dinv::Task* task = new ep3dinv::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}