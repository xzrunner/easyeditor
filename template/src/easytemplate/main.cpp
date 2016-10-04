#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

#include <easytemplate.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyTemplate", ee::SymbolFile::Instance()->Tag(s2::SYM_DUMMY));
	etemplate::Task* task = new etemplate::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}