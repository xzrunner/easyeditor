#include "main.h"
#include "Task.h"
#include "Frame.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easyejoy2d.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyshape.h>
#include <easyicon.h>
#include <easyshadow.h>
#include <easysketch.h>
#include <easytext.h>

IMPLEMENT_APP(MyApp)

static void InitSymbolCreators() 
{
	ee::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eejoy2d::FILE_TAG, &eejoy2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eejoy2d::FILE_TAG, &eejoy2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(eshadow::FILE_TAG, &eshadow::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(eshadow::FILE_TAG, &eshadow::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(libsketch::FILE_TAG, &libsketch::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(libsketch::FILE_TAG, &libsketch::Sprite::Create);

	ee::SymbolFactory::RegisterCreator(etext::FILE_TAG, &etext::Symbol::Create);
	ee::SpriteFactory::Instance()->RegisterCreator(etext::FILE_TAG, &etext::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	ee::Frame* frame = new ecomplex::Frame("EasyComplex", ecomplex::FILE_TAG);
	ecomplex::Task* task = new ecomplex::Task(frame);
	frame->SetTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		const_cast<ee::EditPanel*>(task->GetEditPanel())->GetCanvas()->SetCurrentCanvas();

		wxString path(wxGetApp().argv[1]);
		frame->InitWithFile(path);
	}

	return true;
}