#include "EditDialog.h"
#include "ResizeBaseOP.h"
#include "FileSaver.h"
#include "ToolbarPanel.h"
#include "Symbol.h"

#include <wx/splitter.h>

#include <ee/EditPanel.h>
#include <ee/DialogStageCanvas.h>
#include <ee/ConfirmDialog.h>
#include <ee/SpriteFactory.h>

namespace escale9
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, Symbol* sym, wxGLContext* glctx)
	: wxDialog(parent, wxID_ANY, "Edit Scale9", wxDefaultPosition,
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_sym(sym)
{
	SetTitle(sym->GetFilepath());
	InitLayout(glctx);
}

void EditDialog::InitLayout(wxGLContext* glctx)
{
	wxSplitterWindow* splitter = new wxSplitterWindow(this);
	
	m_stage = new ee::EditPanel(splitter, this);
	m_stage->SetEditOP(new ResizeBaseOP(m_stage, m_stage->GetStageImpl(), m_sym));
	m_stage->SetCanvas(new ee::DialogStageCanvas(m_stage, m_stage->GetStageImpl(), m_sym, glctx));

	ToolbarPanel* toolbar = new ToolbarPanel(splitter, m_stage, m_sym);
	
	splitter->SetSashGravity(0.85f);
	splitter->SplitVertically(m_stage, toolbar);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES)
	{
		const std::string& filepath = m_sym->GetFilepath();
		FileSaver::Store(filepath.c_str(), *m_sym);
		m_sym->RefreshThumbnail(filepath);
		ee::SpriteFactory::Instance()->UpdateBoundings(*m_sym);
		Destroy();
	}
	else if (val == wxID_NO)
	{
		m_sym->LoadFromFile(m_sym->GetFilepath());
		Destroy();
	}
}

}