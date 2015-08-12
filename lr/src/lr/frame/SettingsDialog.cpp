#include "SettingsDialog.h"
#include "SettingCfg.h"

#include "view/StagePanel.h"

#include <easyterrain2d.h>

namespace lr
{

SettingDialog::SettingDialog(wxWindow* parent, StagePanel* stage)
	: wxDialog(parent, wxID_ANY, wxT("Settings"))
	, m_stage(stage)
{
	InitLayout();
}

void SettingDialog::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);

	SettingCfg* cfg = SettingCfg::Instance();
	// Map
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Map"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "width"));

			m_map_width_ctrl = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(cfg->m_map_width), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_map_width_ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SettingDialog::OnMapSizeChanged));
			sz->Add(m_map_width_ctrl);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "height"));

			m_map_height_ctrl = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(cfg->m_map_height), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_map_height_ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SettingDialog::OnMapSizeChanged));
			sz->Add(m_map_height_ctrl);

			sizer->Add(sz);
		}
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);
	// View
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("View"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "width"));

			m_view_width_ctrl = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(cfg->m_view_width), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_view_width_ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_view_width_ctrl);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "height"));

			m_view_height_ctrl = new wxTextCtrl(this, wxID_ANY, wxString::FromDouble(cfg->m_view_height), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_view_height_ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_view_height_ctrl);

			sizer->Add(sz);
		}
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(10);

	wxSizer* right_sizer = new wxBoxSizer(wxVERTICAL);
	// Open River Animation
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("terrain2d动画"));
		check->SetValue(cfg->m_terrain2d_anim);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingDialog::OnChangeTerrain2DAnim));
		right_sizer->Add(check);
	}
	// All Layers Visible&Editable
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("所有层可视可编辑"));
		check->SetValue(cfg->m_all_layers_visible_editable);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingDialog::OnChangeAllLayersVisibleEditable));
		right_sizer->Add(check);
	}
	// Open Layer Draw Flag
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("特殊层标识"));
		check->SetValue(cfg->m_special_layer_flag);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(SettingDialog::OnChangeSpecialLayerFlag));
		right_sizer->Add(check);
	}
	top_sizer->Add(right_sizer);

	SetSizer(top_sizer);

	top_sizer->Layout();
	Refresh(true);
}

void SettingDialog::OnMapSizeChanged(wxCommandEvent& event)
{
	double width, height;
	m_map_width_ctrl->GetValue().ToDouble(&width);
	m_map_height_ctrl->GetValue().ToDouble(&height);

	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_map_width = width;
	cfg->m_map_height = height;
	m_stage->BuildGrids(cfg->m_map_width, cfg->m_map_height);
}

void SettingDialog::OnViewSizeChanged(wxCommandEvent& event)
{
	double width, height;
	m_view_width_ctrl->GetValue().ToDouble(&width);
	m_view_height_ctrl->GetValue().ToDouble(&height);

	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_view_width = width;
	cfg->m_view_height = height;
}

void SettingDialog::OnChangeTerrain2DAnim(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_terrain2d_anim = event.IsChecked();

	std::vector<d2d::ISprite*> sprites;
	m_stage->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		if (eterrain2d::Sprite* terr = dynamic_cast<eterrain2d::Sprite*>(sprites[i])) {
			const_cast<eterrain2d::Symbol&>(terr->GetSymbol()).SetUpdateOpen(cfg->m_terrain2d_anim);
		}
	}
}

void SettingDialog::OnChangeAllLayersVisibleEditable(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_all_layers_visible_editable = event.IsChecked();
}

void SettingDialog::OnChangeSpecialLayerFlag(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_special_layer_flag = event.IsChecked();	
	m_stage->SetCanvasDirty();
}

}