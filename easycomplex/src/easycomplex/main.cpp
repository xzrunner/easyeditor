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
	d2d::SymbolFactory::RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(ecomplex::FILE_TAG, &ecomplex::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libanim::FILE_TAG, &libanim::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libanim::FILE_TAG, &libanim::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(escale9::FILE_TAG, &escale9::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(escale9::FILE_TAG, &escale9::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(emesh::FILE_TAG, &emesh::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(emesh::FILE_TAG, &emesh::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eparticle3d::FILE_TAG, &eparticle3d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eparticle2d::FILE_TAG, &eparticle2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eejoy2d::FILE_TAG, &eejoy2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eejoy2d::FILE_TAG, &eejoy2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eterrain2d::FILE_TAG, &eterrain2d::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(etexture::FILE_TAG, &etexture::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etexture::FILE_TAG, &etexture::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libshape::FILE_TAG, &libshape::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libshape::FILE_TAG, &libshape::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eicon::FILE_TAG, &eicon::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eicon::FILE_TAG, &eicon::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(eshadow::FILE_TAG, &eshadow::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(eshadow::FILE_TAG, &eshadow::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(libsketch::FILE_TAG, &libsketch::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(libsketch::FILE_TAG, &libsketch::Sprite::Create);

	d2d::SymbolFactory::RegisterCreator(etext::FILE_TAG, &etext::Symbol::Create);
	d2d::SpriteFactory::Instance()->RegisterCreator(etext::FILE_TAG, &etext::Sprite::Create);
}

bool MyApp::OnInit()
{
	InitSymbolCreators();

	d2d::Frame* frame = new ecomplex::Frame("EasyComplex", ecomplex::FILE_TAG);
	ecomplex::Task* task = new ecomplex::Task(frame);
	frame->setTask(task);
	frame->Show(true);

	if (wxGetApp().argc > 1) {
		const_cast<d2d::EditPanel*>(task->GetEditPanel())->GetCanvas()->SetCurrentCanvas();

		wxString path(wxGetApp().argv[1]);
		frame->initWithFile(path);
	}

	return true;
}