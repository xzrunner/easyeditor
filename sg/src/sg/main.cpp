#include "main.h"
#include "Task.h"
#include "config.h"
#include "Frame.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new sg::Frame("SgEditor", sg::FILE_TAG);
	sg::Task* task = new sg::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
// 		wxString dir(wxGetApp().argv[1]);
// 
// 		wxArrayString files;
// 		d2d::FilenameTools::fetchAllFiles(dir.ToStdString(), files);
// 		for (int i = 0, n = files.size(); i < n; ++i) 
// 		{
// 			wxFileName filename(files[i]);
// 			filename.Normalize();
// 			wxString filepath = filename.GetFullPath();
// 			if (filepath.Contains("_sg.json")) {
// 				int zz = 0;
// 				task->load(filepath);
// 				task->store(filepath);
// 			}
// 		}
	}

	return true;
}