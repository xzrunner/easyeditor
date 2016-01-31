#include "PreviewDialog.h"
#include "PreviewCanvas.h"

namespace coceditor
{

PreviewDialog::PreviewDialog(wxWindow* parent, ee::LibraryPanel* library,
							 const std::vector<ee::ISprite*>& sprites)
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
	m_stage = new ee::EditPanel(this, this);
	m_stage->setEditOP(new ee::ZoomViewOP(m_stage, false));
	m_stage->setCanvas(new PreviewCanvas(m_stage, m_library, m_sprites));
	topSizer->Add(m_stage, 1, wxEXPAND);
}

} // coceditor