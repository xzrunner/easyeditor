#include "main.h"
#include "Task.h"

#include <ee/Frame.h>
#include <ee/SymbolFile.h>

#include <easyshadow.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyShadow", ee::SymbolFile::Instance()->Tag(ee::SYM_SHADOW));
	eshadow::Task* task = new eshadow::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}