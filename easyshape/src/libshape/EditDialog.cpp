#include "EditDialog.h"
#include "Symbol.h"
#include "config.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <wx/splitter.h>

namespace libshape
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

	wxString filepath = d2d::FilenameTools::getFilenameAddTag(
		symbol->GetFilepath(), libshape::FILE_TAG, "json");
	if (d2d::FilenameTools::IsFileExist(filepath)) {
		m_stage->LoadFromFile(filepath.mb_str());
		m_toolbar->SelectSuitableEditOP();
	}

	m_stage->SetTitleStatus(true);
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

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(horizontal);
	m_stage = new StagePanel(vertical, this, symbol);
	property->SetEditPanel(m_stage->GetStageImpl());
	m_toolbar = new ToolbarPanel(horizontal, property, NULL, m_stage);

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

	d2d::ISymbol& symbol = const_cast<d2d::ISymbol&>(m_stage->GetSymbol());
	const std::string& filepath = symbol.GetFilepath();

	d2d::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		static_cast<Symbol&>(symbol).StoreToFile(filepath.c_str());
		symbol.RefreshThumbnail(filepath);
		d2d::SpriteFactory::Instance()->updateBoundings(symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		symbol.LoadFromFile(filepath);
		Destroy();
	}
}

}