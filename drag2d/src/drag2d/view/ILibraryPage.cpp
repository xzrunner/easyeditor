#include "ILibraryPage.h"
#include "LibraryList.h"
#include "IStageCanvas.h"
#include "ExceptionDlg.h"

#include "common/Config.h"
#include "common/Exception.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ISymbol.h"
#include "message/ResetViewportSJ.h"

namespace d2d
{

ILibraryPage::ILibraryPage(wxWindow* parent, const wxString& name,
						   bool isStatic)
	: wxWindow(parent, wxID_ANY)
	, m_name(name)
	, m_is_static(isStatic)
	, m_list(NULL)
{
	m_btn_add = m_btn_del = NULL;
}

void ILibraryPage::Clear()
{
	m_list->Clear();
}

void ILibraryPage::Traverse(IVisitor& visitor) const
{
	m_list->Traverse(visitor);
}

ListItem* ILibraryPage::GetItem(int index/* = -1*/) const
{
	return m_list->GetItem(index);
}

ISymbol* ILibraryPage::GetSymbol(int index/* = -1*/) const
{
	ListItem* item = GetItem(index);
	if (item) {
		return static_cast<ISymbol*>(item);
	} else {
		return NULL;
	}
}

void ILibraryPage::ReloadTexture() const
{
	m_list->ReloadTexture();
}

void ILibraryPage::AddItem(ListItem* item)
{
	m_list->Insert(item);
//	ResetViewportSJ::Instance()->Reset();
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
	m_list->Remove();
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

bool ILibraryPage::LoadFromConfig(const std::string& key)
{
	bool ret = false;
	std::vector<std::string> filenames;
	Config::Instance()->GetStrings(key, filenames);
	for (int i = 0, n = filenames.size(); i < n; ++i)
	{
		std::string filename = filenames[i];
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filename);
		m_list->Insert(symbol);
		symbol->Release();
		ret = true;
	}
	return ret;
}

void ILibraryPage::OnAddPress(const std::string& type)
{
	wxString filter = wxT("*_") + type + wxT(".json");
	wxString msg = wxT("open ") + type;
	wxFileDialog dlg(this, msg, wxEmptyString, wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			try {
				d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filenames[i].ToStdString());
				m_list->Insert(symbol);
				symbol->Release();
			} catch (d2d::Exception& e) {
				d2d::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

} // d2d