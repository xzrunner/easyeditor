
#include "ChangeImage.h"

#include "view/MediaPage.h"
#include "view/ListObserver.h"

using namespace ebuilder;

ChangeImage::ChangeImage(Actor* actor, d2d::LibraryList* mediaList) 
	: Behavior(e_ChangeImage, actor)
	, m_mediaPage(NULL)
{
	selected = mediaList->getSymbol(0);
}

ChangeImage::~ChangeImage() 
{
	if (m_mediaPage)
		m_mediaPage->getListObserver()->removeListener(this);
}

void ChangeImage::update(const d2d::LibraryList& list) 
{
	if (!isSymbolInList(selected, list))
		selected = list.getSymbol(0);
}

void ChangeImage::registerListener(MediaPage* mediaPage) 
{
	m_mediaPage = mediaPage;
	mediaPage->getListObserver()->registerListener(this);
}
