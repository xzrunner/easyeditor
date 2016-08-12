#include "LibraryPage.h"
#include "LibraryList.h"
#include "StageCanvas.h"
#include "ExceptionDlg.h"
#include "Config.h"
#include "Exception.h"
#include "SymbolMgr.h"
#include "Symbol.h"

namespace ee
{

LibraryPage::LibraryPage(wxWindow* parent, const std::string& name,
						 bool isStatic)
	: wxWindow(parent, wxID_ANY)
	, m_name(name)
	, m_is_static(isStatic)
	, m_list(NULL)
{
	m_btn_add = m_btn_del = NULL;
}

void LibraryPage::Clear()
{
	m_list->Clear();
}

void LibraryPage::Traverse(Visitor<ListItem>& visitor) const
{
	m_list->Traverse(visitor);
}

ListItem* LibraryPage::GetItem(int index/* = -1*/) const
{
	return m_list->GetItem(index);
}

Symbol* LibraryPage::GetSymbol(int index/* = -1*/) const
{
	ListItem* item = GetItem(index);
	if (item) {
		return static_cast<Symbol*>(item);
	} else {
		return NULL;
	}
}

void LibraryPage::ReloadTexture() const
{
	m_list->ReloadTexture();
}

void LibraryPage::AddItem(ListItem* item)
{
	m_list->Insert(item);
}

void LibraryPage::InitLayout(bool draggable/* = true*/)
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

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
}

void LibraryPage::OnDelPress(wxCommandEvent& event)
{
	m_list->Remove();
}

void LibraryPage::InitButtons(wxSizer* sizer)
{
	wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	InitLayoutExtend(btnSizer);

	m_btn_add = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btn_add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryPage::OnAddPress));
	btnSizer->Add(m_btn_add, 0, wxLEFT | wxRIGHT, 5);

	m_btn_del = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btn_del->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryPage::OnDelPress));
	btnSizer->Add(m_btn_del, 0, wxLEFT | wxRIGHT, 5);

	sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
}

bool LibraryPage::LoadFromConfig(const std::string& key)
{
	bool ret = false;
	std::vector<std::string> filenames;
	Config::Instance()->GetStrings(key, filenames);
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i];
		Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		m_list->Insert(symbol);
		symbol->RemoveReference();
		ret = true;
	}
	return ret;
}

void LibraryPage::OnAddPress(const std::string& type)
{
	std::string filter = wxT("*_") + type + wxT(".json");
	std::string msg = wxT("open ") + type;
	wxFileDialog dlg(this, msg, wxEmptyString, wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			try {
				Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filenames[i].ToStdString());
				m_list->Insert(symbol);
				symbol->RemoveReference();
			} catch (Exception& e) {
				ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

}