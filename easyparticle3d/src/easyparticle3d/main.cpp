#include "main.h"

#include "Frame.h"
#include "Task.h"

#include <ee/SymbolFactory.h>
#include <ee/SpriteFactory.h>
#include <ee/SymbolFile.h>
#include <ee/SymbolType.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easyparticle3d.h>
#include <easytrail.h>
#include <easymask.h>

#include <sprite2/SymType.h>

#include <wx/image.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_COMPLEX, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_ANIMATION, &eanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_ANIMATION, &eanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_SCALE9, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_SCALE9, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_PARTICLE3D, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(s2::SYM_TRAIL, &etrail::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(s2::SYM_TRAIL, &etrail::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(ee::SymbolFile::Instance()->Tag(s2::SYM_MASK), &emask::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ee::SymbolFile::Instance()->Tag(s2::SYM_MASK), &emask::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	eparticle3d::Frame* frame = new eparticle3d::Frame("EasyParticle3D", ee::SymbolFile::Instance()->Tag(s2::SYM_PARTICLE3D));
	eparticle3d::Task* task = new eparticle3d::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	return true;
}