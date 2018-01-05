#include "main.h"
#include "Task.h"

#include <ee/Frame.h>

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	auto frame = new ee::Frame("EasyTerrain3D", "terrain3d");
	eterrain3d::Task* task = new eterrain3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}