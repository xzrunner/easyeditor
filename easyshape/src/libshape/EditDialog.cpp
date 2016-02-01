#include "EditDialog.h"
#include "Symbol.h"
#include "config.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <ee/FileHelper.h>
#include <ee/panel_msg.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>
#include <ee/PropertySettingPanel.h>

#include <wx/splitter.h>

namespace eshape
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(NULL)
{
	InitLayout(symbol);

	std::string filepath = ee::FileHelper::GetFilenameAddTag(
		symbol->GetFilepath(), eshape::FILE_TAG, "json");
	if (ee::FileHelper::IsFileExist(filepath)) {
		m_stage->LoadFromFile(filepath.c_str());
		m_toolbar->SelectSuitableEditOP();
	}

	ee::SetWndDirtySJ::Instance()->SetDirty();
}

EditDialog::~EditDialog()
{
//	m_stage->storeShapes();
}

void EditDialog::onSize(wxSizeEvent& event)
{
	Layout();
	Refresh(true);
}

void EditDialog::InitLayout(Symbol* symbol)
{
	wxSplitterWindow* vertical = new wxSplitterWindow(this);
	wxSplitterWindow* horizontal = new wxSplitterWindow(vertical);

	ee::PropertySettingPanel* property = new ee::PropertySettingPanel(horizontal);
	m_stage = new StagePanel(vertical, this, symbol);
	property->SetEditPanel(m_stage->GetStageImpl());
	m_toolbar = new ToolbarPanel(horizontal, property, m_stage);

	horizontal->SetSashGravity(0.6f);
	horizontal->SplitHorizontally(m_toolbar, property);

	vertical->SetSashGravity(0.17f);
	vertical->SplitVertically(horizontal, m_stage);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	ee::Symbol& symbol = const_cast<ee::Symbol&>(m_stage->GetSymbol());
	const std::string& filepath = symbol.GetFilepath();

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		static_cast<Symbol&>(symbol).StoreToFile(filepath.c_str());
		symbol.RefreshThumbnail(filepath);
		ee::SpriteFactory::Instance()->UpdateBoundings(symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		symbol.LoadFromFile(filepath);
		Destroy();
	}
}

}