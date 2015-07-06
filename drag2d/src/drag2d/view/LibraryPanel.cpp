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
	InitLayout();

	SetDropTarget(new LibraryPanelDropTarget(this));
}

void LibraryPanel::OnPageChanged(wxBookCtrlEvent& event)
{
	m_selected = m_pages[event.GetSelection()];
	m_selected->OnActive();
}

void LibraryPanel::Clear()
{
	for (size_t i = 0, n = m_pages.size(); i < n; ++i)
		m_pages[i]->Clear();
	LoadFromConfig();
}

void LibraryPanel::ReloadTexture() const
{
	for (size_t i = 0, n = m_pages.size(); i < n; ++i)
		m_pages[i]->ReloadTexture();
}

void LibraryPanel::AddPage(ILibraryPage* page, const char* name)
{
	if (m_notebook->GetPageCount() == 0)
		m_selected = page;

	if (name != NULL) {
		m_notebook->AddPage(page, name);
	} else {
		m_notebook->AddPage(page, page->GetPageName());
	}
	m_pages.push_back(page);
}

ISymbol* LibraryPanel::GetSymbol(int index/* = -1*/) const
{
	if (m_selected)
		return m_selected->GetSymbol(index);
	else
		return NULL;
}

void LibraryPanel::LoadFromSymbolMgr(const SymbolMgr& mgr)
{
	std::vector<ISymbol*> symbols;
	mgr.traverse(FetchAllVisitor<ISymbol>(symbols));
	for (size_t i = 0, n = symbols.size(); i < n; ++i) {
		LoadSymbol(symbols[i]);
	}
}

void LibraryPanel::LoadSymbol(d2d::ISymbol* symbol)
{
	for (size_t j = 0, m = m_pages.size(); j < m; ++j)
	{
		ILibraryPage* page = m_pages[j];
		if (page->IsHandleSymbol(symbol))
		{
			symbol->RefreshThumbnail(symbol->GetFilepath());
			page->GetList()->insert(symbol);
			break;
		}
	}
}

void LibraryPanel::Traverse(IVisitor& visitor) const
{
	if (m_selected)
		m_selected->Traverse(visitor);
}

void LibraryPanel::SetCanvas(GLCanvas* canvas)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i]->SetCanvas(canvas);
	}
}

bool LibraryPanel::AddSymbol(ISymbol* symbol)
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) 
	{
		ILibraryPage* page = m_pages[i];
		if (page->IsHandleSymbol(symbol)) {
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

void LibraryPanel::SetCurrPage(int idx)
{
	if (idx >= 0 && idx < m_pages.size()) {
		m_notebook->SetSelection(idx);
	}
}

void LibraryPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_notebook = new wxNotebook(this, wxID_ANY);
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(LibraryPanel::OnPageChanged));

	sizer->Add(m_notebook, 1, wxEXPAND);

	SetSizer(sizer);
}

} // d2d