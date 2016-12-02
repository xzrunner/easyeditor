#include "main.h"

#include "Frame.h"
#include "Task.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easymask.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easytrail.h>
#include <easyicon.h>
#include <easytext.h>
#include <easyskeleton.h>

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &libanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SCALE9, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SCALE9, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MESH, &emesh::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MESH, &emesh::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_MASK, &emask::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_MASK, &emask::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE2D, &eparticle2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ICON, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ICON, &eicon::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TEXTBOX, &etext::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TEXTBOX, &etext::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TRAIL, &etrail::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TRAIL, &etrail::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SKELETON, &libskeleton::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SKELETON, &libskeleton::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	eanim::Frame* frame = new eanim::Frame("easyanim", ee::SymbolFile::Instance()->Tag(s2::SYM_ANIMATION));
	eanim::Task* task = new eanim::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		const_cast<ee::EditPanel*>(task->GetEditPanel())->GetCanvas()->SetCurrentCanvas();

		std::string path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
	}

	return true;
}