#include "main.h"
#include "Task.h"

#include <ee/SymbolType.h>
#include <ee/SymbolFile.h>

#include <easyterrain2d.h>

#include <ee/Frame.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	ee::Frame* frame = new ee::Frame("EasyTerrain2D", ee::SymbolFile::Instance()->Tag(ee::SYM_TERRAIN2D));
	eterrain2d::Task* task = new eterrain2d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}