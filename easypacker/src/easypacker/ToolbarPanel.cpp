#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "ImageToolbarPage.h"
#include "MeshToolbarPage.h"
#include "RegularToolbarPage.h"

namespace epacker
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
	, m_stage(stage)
{
	SetSizer(initLayout());
}

IMG_TYPE ToolbarPanel::getImgType() const
{
	return m_image_page->getImgType();
}

void ToolbarPanel::setSize(int width, int height)
{
	m_image_page->setSize(width, height);
}

wxSizer* ToolbarPanel::initLayout()
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