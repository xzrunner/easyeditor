#include "EditDialog.h"
#include "StagePanel.h"
#include "FileSaver.h"

namespace libcomplex
{
	BEGIN_EVENT_TABLE(EditDialog, wxDialog)
		EVT_CLOSE(EditDialog::onClose)
	END_EVENT_TABLE()

	EditDialog::EditDialog(wxWindow* parent, d2d::ComplexSymbol* symbol,
		d2d::PropertySettingPanel* propertyPanel)
 		: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition, 
		wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
		, m_symbol(symbol)
		, m_propertyPanel(propertyPanel)
		, m_editPanel(NULL)
	{
		SetTitle(symbol->getFilepath());
		initLayout();
	}

	void EditDialog::initLayout()
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		m_editPanel = new StagePanel(this, this, m_symbol);
		sizer->Add(m_editPanel, 5, wxEXPAND);
		
		SetSizer(sizer);
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
			FileSaver::store(m_symbol->getFilepath(), m_symbol);

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