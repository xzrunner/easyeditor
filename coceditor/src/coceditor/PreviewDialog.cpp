#include "PreviewDialog.h"
#include "PreviewCanvas.h"

namespace coceditor
{

PreviewDialog::PreviewDialog(wxWindow* parent, d2d::LibraryPanel* library,
							 const std::vector<d2d::ISprite*>& sprites)
 	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_library(library)
	, m_sprites(sprites)
{
	initLayout();
}

PreviewDialog::~PreviewDialog()
{
//	delete m_stage;
}

void PreviewDialog::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	buildEditPanel(sizer);
	SetSizer(sizer);
}

void PreviewDialog::buildEditPanel(wxSizer* topSizer)
{
	m_stage = new d2d::EditPanel(this, this);
	m_stage->setEditOP(new d2d::ZoomViewOP(m_stage, false));
	m_stage->setCanvas(new PreviewCanvas(m_stage, m_library, m_sprites));
	topSizer->Add(m_stage, 1, wxEXPAND);
}

} // coceditor