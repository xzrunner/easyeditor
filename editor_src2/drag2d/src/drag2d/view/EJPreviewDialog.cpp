#include "EJPreviewDialog.h"
#include "EJPreviewCanvas.h"
#include "LibraryPanel.h"
#include "EditPanel.h"

#include "dataset/ISprite.h"
#include "operator/ZoomViewOP.h"

namespace d2d
{

EJPreviewDialog::EJPreviewDialog(wxWindow* parent, /*LibraryPanel* library,*/
	const std::vector<const ISprite*>& sprites)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_sprites(sprites)
{
	initLayout();
}

EJPreviewDialog::~EJPreviewDialog()
{
}

void EJPreviewDialog::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new EditPanel(this, this);
	m_stage->setEditOP(new ZoomViewOP(m_stage, false));
	m_stage->setCanvas(new EJPreviewCanvas(m_stage, m_sprites));
	sizer->Add(m_stage, 1, wxEXPAND);

	SetSizer(sizer);
}

}