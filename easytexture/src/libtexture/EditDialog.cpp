#include "EditDialog.h"
#include "Symbol.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "FileSaver.h"

#include <wx/splitter.h>

namespace etexture
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnClose)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(NULL)
	, m_stage(NULL)
{
	if (symbol) {
		symbol->Retain();
		m_symbol = symbol;
	}
	SetTitle(symbol->getFilepath());
	InitLayout();
}

EditDialog::~EditDialog()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

void EditDialog::InitLayout()
{
	wxSplitterWindow* vert_splitter = new wxSplitterWindow(this);
//	wxSplitterWindow* hori_splitter = new wxSplitterWindow(vert_splitter);

	LibraryPanel* library = new LibraryPanel(vert_splitter);
	StagePanel* stage = new StagePanel(vert_splitter, this, m_symbol, library);
	m_stage = stage;
	vert_splitter->SetSashGravity(0.15f);
	vert_splitter->SplitVertically(library, stage);
}

void EditDialog::OnClose(wxCloseEvent& event)
{
	if (m_stage->isDirty())
	{
		d2d::ExitDlg dlg(this);
		int val = dlg.ShowModal();
		if (val == wxID_OK)
		{
			const wxString& filepath = m_symbol->getFilepath();
			FileSaver::Store(filepath, m_symbol);
			m_symbol->RefreshThumbnail(filepath);
			d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
		}
		else if (val == wxID_CANCEL)
		{
			m_symbol->loadFromFile(m_symbol->getFilepath());
		}
	}

	if(IsModal()) {
		EndModal(wxID_CANCEL);
	} else {
		SetReturnCode(wxID_CANCEL);
		Hide();
	}

	if (!event.CanVeto()) {
		Destroy();
	}
}

}