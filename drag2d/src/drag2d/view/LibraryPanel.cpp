#include "LibraryPanel.h"
#include "ILibraryPage.h"
#include "LibraryList.h"
#include "LibraryImagePage.h"
#include "LibraryPanelDropTarget.h"

#include "common/visitors.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ImageSymbol.h"

namespace d2d
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_selected(NULL)
{
	initLayout();

	SetDropTarget(new LibraryPanelDropTarget(this));
}

void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
{
	m_selected = m_pages[event.GetSelection()];
}

void LibraryPanel::clear()
{
	for (size_t i = 0, n = m_pages.size(); i < n; ++i)
		m_pages[i]->clear();
	LoadFromConfig();
}

void LibraryPanel::reloadTexture() const
{
	for (size_t i = 0, n = m_pages.size(); i < n; ++i)
		m_pages[i]->reloadTexture();
}

void LibraryPanel::addPage(ILibraryPage* page, const char* name)
{
	if (m_notebook->GetPageCount() == 0)
		m_selected = page;

	if (name != NULL) {
		m_notebook->AddPage(page, name);
	} else {
		m_notebook->AddPage(page, page->getName());
	}
	m_pages.push_back(page);
}

ISymbol* LibraryPanel::getSymbol(int index/* = -1*/) const
{
	if (m_selected)
		return m_selected->getSymbol(index);
	else
		return NULL;
}

void LibraryPanel::loadFromSymbolMgr(const SymbolMgr& mgr)
{
	std::vector<ISymbol*> symbols;
	mgr.traverse(FetchAllVisitor<ISymbol>(symbols));
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		ISymbol* symbol = symbols[i];
		for (size_t j = 0, m = m_pages.size(); j < m; ++j)
		{
			ILibraryPage* page = m_pages[j];
			if (page->isHandleSymbol(symbol))
			{
				symbol->RefreshThumbnail(symbol->getFilepath());
				page->getList()->insert(symbol);
				break;
			}
		}
	}
}

void LibraryPanel::traverse(IVisitor& visitor) const
{
	if (m_selected)
		m_selected->traverse(visitor);
}

void LibraryPanel::setCanvas(GLCanvas* canvas)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->setCanvas(canvas);
	}
}

bool LibraryPanel::AddSymbol(ISymbol* symbol)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) 
	{
		ILibraryPage* page = m_pages[i];
		if (page->isHandleSymbol(symbol)) {
			page->AddItem(symbol);
			return true;
		}
	}
	return false;
}

void LibraryPanel::LoadFromConfig()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->LoadFromConfig();
	}
}

void LibraryPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_notebook = new wxNotebook(this, wxID_ANY);
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(LibraryPanel::onPageChanged));

	sizer->Add(m_notebook, 1, wxEXPAND);

	SetSizer(sizer);
}

} // d2d