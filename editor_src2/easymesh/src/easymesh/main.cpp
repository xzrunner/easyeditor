#include "main.h"

#include <drag2d.h>

#include "Task.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	d2d::Frame* frame = new d2d::Frame("EasyMesh", "mesh");
	emesh::Task* task = new emesh::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}