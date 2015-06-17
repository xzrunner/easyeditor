#include "PreviewDialog.h"
#include "PreviewCanvas.h"
#include "PreviewEditOP.h"

namespace ecomplex
{

PreviewDialog::PreviewDialog(wxWindow* parent,
							 const std::vector<const d2d::ISprite*>& sprites)
	: wxDialog(parent, wxID_ANY, "Preview", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, m_sprites(sprites)
	, m_control(0.033f)
{
	InitLayout();

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	data.particle3d_loop = false;
}

PreviewDialog::~PreviewDialog()
{
	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	data.particle3d_loop = true;
}

void PreviewDialog::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	BuildEditPanel(sizer);
	SetSizer(sizer);
}

void PreviewDialog::BuildEditPanel(wxSizer* sizer)
{
	d2d::EditPanel* stage = new d2d::EditPanel(this, this);
	stage->setEditOP(new PreviewEditOP(stage, m_sprites));
	stage->SetCanvas(new PreviewCanvas(stage, m_control, m_sprites));
	sizer->Add(stage, 1, wxEXPAND);
}

}