#include "ILibraryPage.h"
#include "LibraryList.h"
#include "IStageCanvas.h"

#include "common/Config.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ISymbol.h"

namespace d2d
{

ILibraryPage::ILibraryPage(wxWindow* parent, const wxString& name,
						   bool isStatic)
	: wxWindow(parent, wxID_ANY)
	, m_name(name)
	, m_is_static(isStatic)
	, m_list(NULL)
	, m_canvas(NULL)
{
	m_btn_add = m_btn_del = NULL;
}

void ILibraryPage::Clear()
{
	m_list->clear();
}

void ILibraryPage::Traverse(IVisitor& visitor) const
{
	m_list->traverse(visitor);
}

ListItem* ILibraryPage::GetItem(int index/* = -1*/) const
{
	return m_list->getItem(index);
}

ISymbol* ILibraryPage::GetSymbol(int index/* = -1*/) const
{
	return m_list->getSymbol(index);
}

void ILibraryPage::ReloadTexture() const
{
	m_list->ReloadTexture();
}

void ILibraryPage::AddItem(ListItem* item)
{
	m_list->insert(item);
	if (m_canvas) {
		m_canvas->ResetViewport();
	}
}

void ILibraryPage::InitLayout(bool draggable/* = true*/)
{
	wxSizer* sizer = GetSizer();
	if (sizer)
		sizer->DeleteWindows();
	else
		sizer = new wxBoxSizer(wxVERTICAL);

	if (!m_is_static) {
//		InitLayoutExtend(sizer);
		InitButtons(sizer);
	}

	if (!m_list) {
		m_list = new LibraryList(this);
	}
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void ILibraryPage::OnAddPress(wxCommandEvent& event)
{
}

void ILibraryPage::OnDelPress(wxCommandEvent& event)
{
	m_list->remove();
}

void ILibraryPage::InitButtons(wxSizer* sizer)
{
	wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	InitLayoutExtend(btnSizer);

	m_btn_add = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ILibraryPage::OnAddPress));
	btnSizer->Add(m_btn_add, 0, wxLEFT | wxRIGHT, 5);

	m_btn_del = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ILibraryPage::OnDelPress));
	btnSizer->Add(m_btn_del, 0, wxLEFT | wxRIGHT, 5);

	sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
}

void ILibraryPage::LoadFromConfig(const std::string& key)
{
	std::vector<std::string> filenames;
	Config::Instance()->GetStrings(key, filenames);
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i];
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		m_list->insert(symbol);
		symbol->Release();
	}
}

} // d2d