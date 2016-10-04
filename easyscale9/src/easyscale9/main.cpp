#include "main.h"
#include "Task.h"

#include <ee/SymbolFile.h>
#include <ee/Frame.h>

#include <easyscale9.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyScale9", ee::SymbolFile::Instance()->Tag(s2::SYM_SCALE9));
	escale9::Task* task = new escale9::Task(frame);
	frame->SetTask(task);
	frame->Show(true);
	return true;
}