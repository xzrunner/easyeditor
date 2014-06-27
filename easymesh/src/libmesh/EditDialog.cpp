#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "Shape.h"

#include <wx/splitter.h>

namespace emesh
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::onClose)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol)
	: wxDialog(parent, wxID_ANY, "Edit Mesh", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_symbol(symbol)
{
	SetTitle(symbol->getFilepath());
	initLayout();

	m_symbol->SetPause(true);
//	m_symbol->getShape()->RefreshTriangles();
}

EditDialog::~EditDialog()
{
	m_symbol->SetPause(false);
}

void EditDialog::initLayout()
{
 	wxSplitterWindow* splitter = new wxSplitterWindow(this);
 
 	StagePanel* stage = new StagePanel(splitter, this);
 	stage->getSprite()->setSymbol(m_symbol);
 	m_stage = stage;
 	d2d::ToolbarPanel* toolbar = new ToolbarPanel(splitter, stage, false);
 
 	splitter->SetSashGravity(0.85f);
 	splitter->SplitVertically(stage, toolbar);
}

void EditDialog::onClose(wxCloseEvent& event)
{
	if (!m_stage->isDirty())
	{
		Destroy();
		return;
	}

	d2d::ExitDlg dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_OK)
	{
		FileIO::store(m_symbol->getFilepath(), m_symbol);

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