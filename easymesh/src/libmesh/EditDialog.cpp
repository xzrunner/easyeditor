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

EditDialog::EditDialog(wxWindow* parent, Sprite* sprite)
	: wxDialog(parent, wxID_ANY, "Edit Mesh", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_sprite(sprite)
{
	Symbol& symbol = const_cast<Symbol&>(m_sprite->getSymbol());
	SetTitle(symbol.GetFilepath());
	initLayout();

	symbol.SetPause(true);
//	m_symbol->getShape()->RefreshTriangles();
}

EditDialog::~EditDialog()
{
	Symbol& symbol = const_cast<Symbol&>(m_sprite->getSymbol());
	symbol.SetPause(false);
}

void EditDialog::initLayout()
{
 	wxSplitterWindow* splitter = new wxSplitterWindow(this);
 
 	StagePanel* stage = new StagePanel(splitter, this);
	Symbol& symbol = const_cast<Symbol&>(m_sprite->getSymbol());
	stage->SetSymbol(&symbol);
 	m_stage = stage;
 	d2d::ToolbarPanel* toolbar = new ToolbarPanel(splitter, stage, false, m_sprite);
 
 	splitter->SetSashGravity(0.85f);
 	splitter->SplitVertically(stage, toolbar);
}

void EditDialog::onClose(wxCloseEvent& event)
{
	m_sprite->buildBounding();

	if (!m_stage->isDirty())
	{
		Destroy();
		return;
	}

	Symbol& symbol = const_cast<Symbol&>(m_sprite->getSymbol());

	d2d::ExitDlg dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_OK)
	{
		const std::string& filepath = symbol.GetFilepath();
		FileIO::store(symbol.GetFilepath().c_str(), &symbol);
		symbol.RefreshThumbnail(filepath);

		d2d::SpriteFactory::Instance()->updateBoundings(symbol);
	}
	else if (val == wxID_CANCEL)
	{
		symbol.LoadFromFile(symbol.GetFilepath());
	}

	Destroy();
}

}