#include "ILibraryPage.h"
#include "LibraryList.h"
#include "GLCanvas.h"

#include "common/Config.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ISymbol.h"

namespace d2d
{

ILibraryPage::ILibraryPage(wxWindow* parent, const wxString& name,
						   bool isStatic)
	: wxWindow(parent, wxID_ANY)
	, m_name(name)
	, m_isStatic(isStatic)
	, m_list(NULL)
	, m_canvas(NULL)
{
	m_btnAdd = m_btnDel = NULL;
}

void ILibraryPage::clear()
{
	m_list->clear();
}

void ILibraryPage::traverse(IVisitor& visitor) const
{
	m_list->traverse(visitor);
}

ListItem* ILibraryPage::getItem(int index/* = -1*/) const
{
	return m_list->getItem(index);
}

ISymbol* ILibraryPage::getSymbol(int index/* = -1*/) const
{
	return m_list->getSymbol(index);
}

void ILibraryPage::reloadTexture() const
{
	m_list->reloadTexture();
}

void ILibraryPage::AddItem(ListItem* item)
{
	m_list->insert(item);
	if (m_canvas) {
		m_canvas->resetViewport();
	}
}

void ILibraryPage::initLayout(bool draggable/* = true*/)
{
	wxSizer* sizer = GetSizer();
	if (sizer)
		sizer->DeleteWindows();
	else
		sizer = new wxBoxSizer(wxVERTICAL);

	if (!m_isStatic) {
		initButtons(sizer);
	}

	if (!m_list) {
		m_list = new LibraryList(this);
	}
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void ILibraryPage::onAddPress(wxCommandEvent& event)
{
}

void ILibraryPage::onDelPress(wxCommandEvent& event)
{
	m_list->remove();
}

void ILibraryPage::initButtons(wxSizer* sizer)
{
	wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	m_btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ILibraryPage::onAddPress));
	btnSizer->Add(m_btnAdd, 0, wxLEFT | wxRIGHT, 5);

	m_btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ILibraryPage::onDelPress));
	btnSizer->Add(m_btnDel, 0, wxLEFT | wxRIGHT, 5);

	sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
}

void ILibraryPage::LoadFromConfig(const std::string& key)
{
	std::vector<std::string> filenames;
	Config::Instance()->GetStrings(key, filenames);
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i];
		ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filename);
		m_list->insert(symbol);
		symbol->Release();
	}
}

} // d2d