#include "LibraryFontBlankPage.h"
#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"

#include "dataset/SymbolMgr.h"
#include "dataset/FontBlankSymbol.h"
#include "common/FileNameParser.h"
#include "common/config.h"

#include <JSON/json.h>

namespace d2d
{

LibraryFontBlankPage::LibraryFontBlankPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("FontBlank"))
{
	m_list = new LibraryFontBlankList(this);
	initLayout();

	LoadFromConfig();
}

bool LibraryFontBlankPage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<FontBlankSymbol*>(symbol) != NULL;
}

void LibraryFontBlankPage::initLayout(bool draggable /*= true*/)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* btnNew = new wxButton(this, wxID_ANY, "new", wxDefaultPosition, wxSize(40, 20));
		Connect(btnNew->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryFontBlankPage::onNewBtnPress));
		btnSizer->Add(btnNew, 0, wxRIGHT, 10);

		initButtons(btnSizer);

		sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
	}
	{
		sizer->Add(m_list, 1, wxEXPAND);
	}
	SetSizer(sizer);
}

void LibraryFontBlankPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_fontblank);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入font blank文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filenames[i]);
			m_list->insert(symbol);
			symbol->release();
		}
	}
}

void LibraryFontBlankPage::onNewBtnPress(wxCommandEvent& event)
{
	FontBlankSymbol* item = new FontBlankSymbol();
	m_list->insert(item);

	FontBlankDialog dlg(this, item);
	dlg.ShowModal();
}

void LibraryFontBlankPage::LoadFromConfig()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(CONFIG_FILEPATH);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Json::Value list = value["library_label"];
	if (list.isNull()) {
		return;
	}

	int i = 0;
	Json::Value item_val = list[i++];
	while (!item_val.isNull()) {
		std::string filepath = item_val.asString();
		
		ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filepath);
		m_list->insert(symbol);
		symbol->release();

		item_val = list[i++];
	}
}

} // d2d