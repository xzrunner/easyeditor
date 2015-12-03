#include "EditDialog.h"
#include "ResizeBaseOP.h"
#include "FileSaver.h"
#include "ToolbarPanel.h"
#include "Symbol.h"

#include <wx/splitter.h>

namespace escale9
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* symbol, wxGLContext* glctx)
	: wxDialog(parent, wxID_ANY, "Edit Scale9", wxDefaultPosition,
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_symbol(symbol)
{
	SetTitle(symbol->GetFilepath());
	InitLayout(glctx);
}

void EditDialog::InitLayout(wxGLContext* glctx)
{
	wxSplitterWindow* splitter = new wxSplitterWindow(this);
	
	m_stage = new d2d::EditPanel(splitter, this);
	m_stage->SetEditOP(new ResizeBaseOP(m_stage, m_stage->GetStageImpl(), m_symbol));
	m_stage->SetCanvas(new d2d::DialogStageCanvas(m_stage, m_stage->GetStageImpl(), m_symbol, glctx));

	ToolbarPanel* toolbar = new ToolbarPanel(splitter, m_stage, m_symbol);
	
	splitter->SetSashGravity(0.85f);
	splitter->SplitVertically(m_stage, toolbar);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	d2d::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		const std::string& filepath = m_symbol->GetFilepath();
		FileSaver::store(filepath.c_str(), *m_symbol);
		m_symbol->RefreshThumbnail(filepath);
		d2d::SpriteFactory::Instance()->updateBoundings(*m_symbol);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		m_symbol->LoadFromFile(m_symbol->GetFilepath());
		Destroy();
	}
}

}