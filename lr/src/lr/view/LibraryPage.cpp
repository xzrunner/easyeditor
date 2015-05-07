#include "LibraryPage.h"
#include "StagePanel.h"

namespace lr
{

LibraryPage::LibraryPage(wxWindow* parent, const char* name,
						 const std::string& key, StagePanel* stage)
	: d2d::ILibraryPage(parent, name)
	, m_key(key)
	, m_stage(stage)
	, m_visible(true)
	, m_editable(true)
	, m_layer(NULL)
{
	initLayout();
}

LibraryPage::~LibraryPage()
{
//	m_stage->GetSymbolsCfg().StoreLibraryPage(m_key, this);
}

bool LibraryPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return true;
}

void LibraryPage::LoadFromFile(const Json::Value& value, const std::string& dir)
{
	Json::Value layer_val = value[m_key];
	int idx = 0;
	Json::Value symbol_val = layer_val["symbol"][idx++];
	while (!symbol_val.isNull()) {
		std::string filepath = dir + "//" + symbol_val.asString();
		
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		symbol->RefreshThumbnail(filepath);
		m_list->insert(symbol);
		symbol->Release();

		symbol_val = layer_val["symbol"][idx++];
	}

	m_visible = layer_val["visible"].isNull() || layer_val["visible"].asBool();
	m_visible_ctrl->SetValue(m_visible);
	if (!m_visible) {
		VisibleAllSprites(false);
	}

	m_editable = layer_val["editable"].isNull() || layer_val["editable"].asBool();
	m_editable_ctrl->SetValue(m_editable);
	if (!m_editable) {
		EditableAllSprites(false);
	}
}

void LibraryPage::StoreToFile(Json::Value& value, const std::string& dir) const
{
	Json::Value& layer_val = value[m_key];
	int idx = 0;
	while (d2d::ISymbol* symbol = m_list->getSymbol(idx)) {
		std::string filepath = d2d::FilenameTools::getRelativePath(dir, symbol->getFilepath());
		layer_val["symbol"][idx] = filepath;
		++idx;
	}

	layer_val["visible"] = m_visible;
	layer_val["editable"] = m_editable;
}

void LibraryPage::InitLayoutExtend(wxSizer* sizer)
{
	{
		m_visible_ctrl = new wxCheckBox(this, wxID_ANY, wxT("可见"));
		Connect(m_visible_ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LibraryPage::OnChangeVisible));
		sizer->Add(m_visible_ctrl, 0, wxALIGN_LEFT);
	}
	{
		m_editable_ctrl = new wxCheckBox(this, wxID_ANY, wxT("编辑"));
		Connect(m_editable_ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(LibraryPage::OnChangeEditable));
		sizer->Add(m_editable_ctrl, 0, wxALIGN_LEFT);
	}
	sizer->AddSpacer(20);
}

void LibraryPage::onAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("导入symbol"), wxEmptyString, 
		wxEmptyString, "*.*", wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			const wxString& filepath = filenames[i];
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			m_list->insert(symbol);
			symbol->Release();
		}

		if (!filenames.empty() && m_canvas) {
			m_canvas->resetViewport();
		}
	}
}

void LibraryPage::OnChangeVisible(wxCommandEvent& event)
{
	bool visible = event.IsChecked();
	if (visible == m_visible) {
		return;
	}

	m_visible = visible;	
	VisibleAllSprites(m_visible);
}

void LibraryPage::OnChangeEditable(wxCommandEvent& event)
{
	bool editable = event.IsChecked();
	if (editable == m_editable) {
		return;
	}

	m_editable = editable;	
	EditableAllSprites(m_editable);
}

void LibraryPage::VisibleAllSprites(bool visible)
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0; i < sprites.size(); ++i) {
		d2d::ISprite* spr = sprites[i];

		bool find = false;
		int idx = 0;
		while (d2d::ISymbol* symbol = m_list->getSymbol(idx++)) {
			if (symbol == &spr->getSymbol()) {
				find = true;
				break;
			}
		}

		if (find) {
			spr->visiable = m_visible;
		}
	}

	m_stage->Refresh();
}

void LibraryPage::EditableAllSprites(bool visible)
{
	std::vector<d2d::ISprite*> sprites;
	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0; i < sprites.size(); ++i) {
		d2d::ISprite* spr = sprites[i];

		bool find = false;
		int idx = 0;
		while (d2d::ISymbol* symbol = m_list->getSymbol(idx++)) {
			if (symbol == &spr->getSymbol()) {
				find = true;
				break;
			}
		}

		if (find) {
			spr->editable = m_editable;
		}
	}

	m_stage->Refresh();
}

}