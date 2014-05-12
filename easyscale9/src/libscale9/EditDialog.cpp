#include "EditDialog.h"
#include "ResizeBaseOP.h"
#include "FileSaver.h"
#include "ToolbarPanel.h"
#include "Symbol.h"

#include <wx/splitter.h>

namespace escale9
{
	BEGIN_EVENT_TABLE(EditDialog, wxDialog)
		EVT_CLOSE(EditDialog::onClose)
	END_EVENT_TABLE()

	EditDialog::EditDialog(wxWindow* parent, Symbol* symbol)
		: wxDialog(parent, wxID_ANY, "Edit Scale9", wxDefaultPosition,
		wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
		, m_symbol(symbol)
	{
		SetTitle(symbol->getFilepath());
		initLayout();
	}

	void EditDialog::initLayout()
	{
		wxSplitterWindow* splitter = new wxSplitterWindow(this);
		
		m_editPanel = new d2d::EditPanel(splitter, this);
		m_editPanel->setEditOP(new ResizeBaseOP(m_editPanel, m_symbol));
		m_editPanel->setCanvas(new d2d::DialogStageCanvas(m_editPanel, m_symbol));

		ToolbarPanel* toolbar = new ToolbarPanel(splitter, m_symbol);
		
		splitter->SetSashGravity(0.85f);
		splitter->SplitVertically(m_editPanel, toolbar);
	}

	void EditDialog::onClose(wxCloseEvent& event)
	{
		if (!m_editPanel->isDirty())
		{
			Destroy();
			return;
		}

		d2d::ExitDlg dlg(this);
		int val = dlg.ShowModal();
		if (val == wxID_OK)
		{
			FileSaver::store(m_symbol->getFilepath(), *m_symbol);

			m_symbol->refresh();
			d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
		}
		else if (val == wxID_CANCEL)
		{
			m_symbol->loadFromFile(m_symbol->getFilepath());
		}

		Destroy();
	}
}