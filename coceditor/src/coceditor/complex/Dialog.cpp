#include "Dialog.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include "LibraryPanel.h"
#include "Context.h"
#include "../StagePanel.h"



namespace coceditor
{
namespace complex
{
	BEGIN_EVENT_TABLE(Dialog, wxDialog)
		EVT_SIZE(Dialog::onSize)
	END_EVENT_TABLE()

	Dialog::Dialog(wxWindow* parent, complex::Symbol* complexSymbol,
					ee::PropertySettingPanel* propertyPanel)
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

	void Dialog::initLayout(complex::Symbol* complexSymbol)
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	//	ee::LibraryPanel* libraryPanel = new ee::LibraryPanel(this);
	//	libraryPanel->addPage(new ee::LibraryImagePage(libraryPanel->getNotebook()));
	//	libraryPanel->loadFromSymbolMgr(*ee::SymbolMgr::Instance());

 		LibraryPanel* libraryPanel = new LibraryPanel(this);
 		{
 			ee::ILibraryPage* page = Context::Instance()->library->getImagePage();
 			int index = 0;
 			while (ee::ISymbol* symbol = page->getSymbol(index++))
 				libraryPanel->getImagePage()->getList()->insert(symbol);
 		}
 		{
 			ee::ILibraryPage* page = Context::Instance()->library->getComplexPage();
 			int index = 0;
 			while (ee::ISymbol* symbol = page->getSymbol(index++))
 				libraryPanel->getComplexPage()->getList()->insert(symbol);			
 		}
//		libraryPanel->loadFromSymbolMgr(*ee::SymbolMgr::Instance());

		sizer->Add(libraryPanel, 1, wxEXPAND);

		m_editPanel = new StagePanel(this, this, libraryPanel, complexSymbol);
		sizer->Add(m_editPanel, 5, wxEXPAND);
		
		sizer->Add(new ToolbarPanel(this, m_editPanel));

		SetSizer(sizer);
	}

	void Dialog::refreshSpritesBounding()
	{
		std::vector<complex::Sprite*> sprites;
		Context::Instance()->stage->traverseSprites(ee::FetchAllVisitor<complex::Sprite>(sprites));
		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		{
			ee::ISprite* sprite = sprites[i];
			if (&sprite->getSymbol() == m_complexSymbol)
				sprite->buildBounding();
		}
	}
}
}