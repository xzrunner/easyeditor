#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <easymesh.h>

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators() ;

	ee::Frame* frame = new emesh::Frame("EasyMesh", emesh::FILE_TAG);
	emesh::Task* task = new emesh::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		std::string path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
	}

	return true;
}