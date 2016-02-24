#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "ImageToolbarPage.h"
#include "MeshToolbarPage.h"
#include "RegularToolbarPage.h"

#include <wx/notebook.h>

namespace etexpacker
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage(stage)
{
	SetSizer(InitLayout());
}

IMG_TYPE ToolbarPanel::GetImgType() const
{
	return m_image_page->GetImgType();
}

void ToolbarPanel::SetSize(int width, int height)
{
	m_image_page->SetSize(width, height);
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxNotebook* notebook = new wxNotebook(this, wxID_ANY);
	sizer->Add(notebook, 1, wxEXPAND);

	m_image_page = new ImageToolbarPage(notebook, m_stage);
	notebook->AddPage(m_image_page, "Image");

	wxPanel* mesh_page = new MeshToolbarPage(notebook, m_stage);
	notebook->AddPage(mesh_page, "Mesh");

	wxPanel* regular_page = new RegularToolbarPage(notebook, m_stage);
	notebook->AddPage(regular_page, "Regular");

	return sizer;
}

}