#include "main.h"
#include "Task.h"
#include "Frame.h"

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

	d2d::Frame* frame = new emesh::Frame("EasyMesh", emesh::FILE_TAG);
	emesh::Task* task = new emesh::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		wxString path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
	}

	return true;
}