#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "Shape.h"

#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>

#include <wx/splitter.h>

namespace emesh
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Sprite* sprite, wxGLContext* glctx)
	: wxDialog(parent, wxID_ANY, "Edit Mesh", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_sprite(sprite)
{
	Symbol& symbol = const_cast<Symbol&>(m_sprite->GetSymbol());
	SetTitle(symbol.GetFilepath());
	InitLayout(glctx);

	symbol.SetPause(true);
//	m_symbol->getShape()->RefreshTriangles();
}

EditDialog::~EditDialog()
{
	Symbol& symbol = const_cast<Symbol&>(m_sprite->GetSymbol());
	symbol.SetPause(false);
}

void EditDialog::InitLayout(wxGLContext* glctx)
{
 	wxSplitterWindow* splitter = new wxSplitterWindow(this);
 
 	StagePanel* stage = new StagePanel(splitter, this, glctx);
	Symbol& symbol = const_cast<Symbol&>(m_sprite->GetSymbol());
	stage->SetSymbol(&symbol);
 	m_stage = stage;
 	ee::ToolbarPanel* toolbar = new ToolbarPanel(splitter, stage, false, m_sprite);
 
 	splitter->SetSashGravity(0.85f);
 	splitter->SplitVertically(stage, toolbar);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	m_sprite->BuildBounding();

	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	Symbol& symbol = const_cast<Symbol&>(m_sprite->GetSymbol());

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		const std::string& filepath = symbol.GetFilepath();
		FileIO::Store(symbol.GetFilepath().c_str(), &symbol);
		symbol.RefreshThumbnail(filepath);

		ee::SpriteFactory::Instance()->UpdateBoundings(symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		symbol.LoadFromFile(symbol.GetFilepath());
		Destroy();
	}
}

}