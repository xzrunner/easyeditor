#include "EditDialog.h"
#include "Symbol.h"
#include "config.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"

#include <wx/splitter.h>

namespace libshape
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnClose)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol)
 	: wxDialog(parent, wxID_ANY, "Edit Shape", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(NULL)
{
	InitLayout(symbol);

	wxString filepath = d2d::FilenameTools::getFilenameAddTag(
		symbol->getFilepath(), libshape::FILE_TAG, "json");
	if (d2d::FilenameTools::isExist(filepath)) {
		m_stage->LoadFromFile(filepath.mb_str());
		m_toolbar->SelectSuitableEditOP();
	}

	m_stage->setTitleStatus(true);
}

EditDialog::~EditDialog()
{
//	m_stage->storeShapes();
}

void EditDialog::onSize(wxSizeEvent& event)
{
	Layout();
	Refresh();
}

void EditDialog::InitLayout(Symbol* symbol)
{
	wxSplitterWindow* vertical = new wxSplitterWindow(this);
	wxSplitterWindow* horizontal = new wxSplitterWindow(vertical);

	d2d::PropertySettingPanel* property = new d2d::PropertySettingPanel(horizontal);
	m_stage = new StagePanel(vertical, this, symbol);
	m_toolbar = new ToolbarPanel(horizontal, property, m_stage);

	horizontal->SetSashGravity(0.6f);
	horizontal->SplitHorizontally(m_toolbar, property);

	vertical->SetSashGravity(0.17f);
	vertical->SplitVertically(horizontal, m_stage);
}

void EditDialog::OnClose(wxCloseEvent& event)
{
	if (!m_stage->isDirty())
	{
		Destroy();
		return;
	}

	d2d::ISymbol& symbol = const_cast<d2d::ISymbol&>(m_stage->GetSymbol());
	const wxString& filepath = symbol.getFilepath();

	d2d::ExitDlg dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_OK)
	{
		static_cast<Symbol&>(symbol).StoreToFile(filepath);
		symbol.RefreshThumbnail(filepath);
		d2d::SpriteFactory::Instance()->updateBoundings(symbol);
	}
	else if (val == wxID_CANCEL)
	{
		symbol.loadFromFile(filepath);
	}

	Destroy();
}

}