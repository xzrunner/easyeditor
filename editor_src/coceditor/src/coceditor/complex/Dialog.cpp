#include "Dialog.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include "LibraryPanel.h"
#include "Context.h"
#include "../StagePanel.h"

#include <drag2d.h>

namespace coceditor
{
namespace complex
{
	BEGIN_EVENT_TABLE(Dialog, wxDialog)
		EVT_SIZE(Dialog::onSize)
	END_EVENT_TABLE()

	Dialog::Dialog(wxWindow* parent, d2d::ComplexSymbol* complexSymbol,
					d2d::PropertySettingPanel* propertyPanel)
 		: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, 
		wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
		, m_complexSymbol(complexSymbol)
		, m_propertyPanel(propertyPanel)
	{
		initLayout(complexSymbol);
	}

	Dialog::~Dialog()
	{
		m_editPanel->storeSymbol();

		m_complexSymbol->refresh();
		refreshSpritesBounding();
	}

	void Dialog::onSize(wxSizeEvent& event)
	{
		Layout();
		Refresh();
	}

	void Dialog::initLayout(d2d::ComplexSymbol* complexSymbol)
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	//	d2d::LibraryPanel* libraryPanel = new d2d::LibraryPanel(this);
	//	libraryPanel->addPage(new d2d::LibraryImagePage(libraryPanel->getNotebook()));
	//	libraryPanel->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());

 		LibraryPanel* libraryPanel = new LibraryPanel(this);
 		{
 			d2d::ILibraryPage* page = Context::Instance()->library->getImagePage();
 			int index = 0;
 			while (d2d::ISymbol* symbol = page->getSymbol(index++))
 				libraryPanel->getImagePage()->getList()->insert(symbol);
 		}
 		{
 			d2d::ILibraryPage* page = Context::Instance()->library->getComplexPage();
 			int index = 0;
 			while (d2d::ISymbol* symbol = page->getSymbol(index++))
 				libraryPanel->getComplexPage()->getList()->insert(symbol);			
 		}
//		libraryPanel->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());

		sizer->Add(libraryPanel, 1, wxEXPAND);

		m_editPanel = new StagePanel(this, this, libraryPanel, complexSymbol);
		sizer->Add(m_editPanel, 5, wxEXPAND);
		
		sizer->Add(new ToolbarPanel(this, m_editPanel));

		SetSizer(sizer);
	}

	void Dialog::refreshSpritesBounding()
	{
		std::vector<d2d::ComplexSprite*> sprites;
		Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ComplexSprite>(sprites));
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			d2d::ISprite* sprite = sprites[i];
			if (&sprite->getSymbol() == m_complexSymbol)
				sprite->buildBounding();
		}
	}
}
}