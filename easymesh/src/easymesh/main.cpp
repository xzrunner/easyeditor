#include "main.h"
#include "Task.h"

#include <easymesh.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators() ;

	d2d::Frame* frame = new d2d::Frame("EasyMesh", emesh::FILE_TAG);
	emesh::Task* task = new emesh::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}