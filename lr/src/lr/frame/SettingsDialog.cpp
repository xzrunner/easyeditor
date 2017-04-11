#include "SettingsDialog.h"
#include "SettingCfg.h"

#include "view/StagePanel.h"

#include <ee/StringHelper.h>
#include <ee/FetchAllVisitor.h>
#include <ee/panel_msg.h>
#include <ee/HSLColorSettingDlg.h>
#include <ee/Image.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>

#include <easyterrain2d.h>
#include <easycomplex.h>

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/ColGradingProg.h>
#include <shaderlab/FilterShader.h>
#include <sprite2/S2_Sprite.h>
#include <sprite2/RFColGrading.h>
#include <gum/Config.h>

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

			m_map_width_ctrl = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(cfg->m_map_width), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_map_width_ctrl->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SettingDialog::OnMapSizeChanged));
			sz->Add(m_map_width_ctrl);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "height"));

			m_map_height_ctrl = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(cfg->m_map_height), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_map_height_ctrl->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SettingDialog::OnMapSizeChanged));
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

			m_view_width_ctrl = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(cfg->m_view_width), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_view_width_ctrl->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_view_width_ctrl);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "height"));

			m_view_height_ctrl = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(cfg->m_view_height), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_view_height_ctrl->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_view_height_ctrl);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "offset x"));

			m_view_offset_x = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(cfg->m_view_dx), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_view_offset_x->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_view_offset_x);

			sizer->Add(sz);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
			sz->Add(new wxStaticText(this, wxID_ANY, "offset y"));

			m_view_offset_y = new wxTextCtrl(this, wxID_ANY, ee::StringHelper::ToString(cfg->m_view_dy), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
			Connect(m_view_offset_y->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SettingDialog::OnViewSizeChanged));
			sz->Add(m_view_offset_y);

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
	// Screen color
	right_sizer->AddSpacer(10);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("屏幕特效"));
 		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		wxButton* btn = new wxButton(this, wxID_ANY, "Open Grading Texture...");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingDialog::OnChangeGradingTexture));
		sizer->Add(btn);

		right_sizer->Add(sizer);

// 		{
// 			wxButton* btn = new wxButton(this, wxID_ANY, "MULTI");
// 			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingDialog::OnChangeScreenMultiColor));
// 			sizer->Add(btn);
// 		}
// 		sizer->AddSpacer(5);
// 		{
// 			wxButton* btn = new wxButton(this, wxID_ANY, "ADD");
// 			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingDialog::OnChangeScreenAddColor));
// 			sizer->Add(btn);
// 		}
// 		right_sizer->Add(sizer);
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

	double dx, dy;
	m_view_offset_x->GetValue().ToDouble(&dx);
	m_view_offset_y->GetValue().ToDouble(&dy);

	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_view_width = width;
	cfg->m_view_height = height;
	cfg->m_view_dx = dx;
	cfg->m_view_dy = dy;
}

void SettingDialog::OnChangeTerrain2DAnim(wxCommandEvent& event)
{
	SettingCfg* cfg = SettingCfg::Instance();
	cfg->m_terrain2d_anim = event.IsChecked();

	std::vector<ee::Sprite*> sprs;
	m_stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		SetTerrain2dUpdate(sprs[i], cfg->m_terrain2d_anim);
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
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void SettingDialog::OnChangeScreenMultiColor(wxCommandEvent& event)
{
	s2::Color& col = m_stage->GetScreenMultiColor();
	ee::HSLColorSettingDlg dlg(this, NULL, col);
	if (dlg.ShowModal() == wxID_OK) {
		col = dlg.GetColor();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void SettingDialog::OnChangeScreenAddColor(wxCommandEvent& event)
{
	s2::Color& col = m_stage->GetScreenAddColor();
	ee::HSLColorSettingDlg dlg(this, NULL, col);
	if (dlg.ShowModal() == wxID_OK) {
		col = dlg.GetColor();
		col.a = 0;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void SettingDialog::OnChangeGradingTexture(wxCommandEvent& event)
{
	wxFileDialog dlg(this, "Open Image", wxEmptyString, wxEmptyString, "*.png");
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filepath = dlg.GetPath().ToStdString();
		
		sl::ColGradingProg* prog = NULL;
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		if (shader) {
			prog = static_cast<sl::ColGradingProg*>(shader->GetProgram(sl::FM_COL_GRADING));
		}
		if (prog) 
		{
			try {
				gum::Config* cfg = gum::Config::Instance();
				bool ori_alpha_cfg = cfg->GetPreMulAlpha();
				cfg->SetPreMulAlpha(false);
				ee::Image* img = ee::ImageMgr::Instance()->GetItem(filepath);
				cfg->SetPreMulAlpha(ori_alpha_cfg);
				if (img) {
					SettingCfg::Instance()->m_post_effect_file = filepath;
					prog->SetLUTTex(img->GetTexID());
					m_stage->EnableColorGrading(true);
				}
			} catch (ee::Exception& e) {
				ee::ExceptionDlg dlg(m_parent, e);
				dlg.ShowModal();
			}
		}
	}
}

void SettingDialog::SetTerrain2dUpdate(ee::Sprite* spr, bool open)
{
	if (eterrain2d::Sprite* terr = dynamic_cast<eterrain2d::Sprite*>(spr)) {
		dynamic_cast<eterrain2d::Symbol*>(terr->GetSymbol())->SetUpdateOpen(open);
	} else if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(spr)) {
		const std::vector<s2::Sprite*>& children = dynamic_cast<s2::ComplexSymbol*>(complex->GetSymbol())->GetAllChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
			SetTerrain2dUpdate(child, open);
		}
	}
}

}