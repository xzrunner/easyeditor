#include "SettingsDialog.h"

#include "common/Config.h"
#include "common/SettingData.h"
#include "widgets/RGBColorSettingDlg.h"

#include <gl/gl.h>

namespace d2d
{

SettingsDialog::SettingsDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Settings"))
	, m_settings(Config::Instance()->GetSettings())
{
	initLayout();
}

void SettingsDialog::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->AddSpacer(10);
	sizer->Add(initEditPanel(), 0, wxCENTER);
	sizer->AddSpacer(10);
	sizer->Add(initImagePanel(), 0, wxCENTER);
	sizer->AddSpacer(10);
	sizer->Add(initViewPanel(), 0, wxCENTER);
	sizer->AddSpacer(10);
	SetSizer(sizer);

	sizer->Layout();
	Refresh();
}

wxSizer* SettingsDialog::initEditPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("±à¼­"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("¶ÔÏó²¶×½"));
		check->SetValue(m_settings.open_sprite_capture);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeSpriteCapture));
		sizer->Add(check, 0);
	}
	return sizer;
}

wxSizer* SettingsDialog::initImagePanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Í¼Æ¬"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("±ßÔµ²Ã¼ô"));
		check->SetValue(m_settings.open_image_edge_clip);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeImageEdgeClip));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("ÏÔÊ¾Ô­Ê¼±ß¿ò"));
		check->SetValue(m_settings.visible_image_edge);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeVisibleImageEdge));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(10);
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("LINEAR FILTER"));
		check->SetValue(m_settings.linear_filter);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(SettingsDialog::onChangeImageFilterType));
		sizer->Add(check, 0);
	}
	return sizer;
}

wxSizer* SettingsDialog::initViewPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("ÏÔÊ¾"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("ÎÄ×Ö¿ò"));
			check->SetValue(m_settings.visible_label_bg);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeFontBackground));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("ÎÄ×ÖÄÚÈÝ"));
			check->SetValue(m_settings.visible_label_text);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeFontText));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("½ÚµãÃû³Æ"));
			check->SetValue(m_settings.visible_node_name);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeNodeName));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("Shape"));
			check->SetValue(m_settings.visible_shape);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeShape));
			sizer->Add(check, 0);
		}
		{
			wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("Texture Edge"));
			check->SetValue(m_settings.visible_tex_edge);
			Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onChangeTextureEdge));
			sizer->Add(check, 0);
		}
		{
			wxButton* btn = new wxButton(this, wxID_ANY, "±³¾°ÑÕÉ«");
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(SettingsDialog::onSetBGColor));
			sizer->Add(btn);
		}
	}
	return sizer;
}

void SettingsDialog::onChangeSpriteCapture(wxCommandEvent& event)
{
	m_settings.open_sprite_capture = event.IsChecked();
}

void SettingsDialog::onChangeImageEdgeClip(wxCommandEvent& event)
{
	m_settings.open_image_edge_clip = event.IsChecked();
}

void SettingsDialog::onChangeVisibleImageEdge(wxCommandEvent& event)
{
	m_settings.visible_image_edge = event.IsChecked();
}

void SettingsDialog::onChangeImageFilterType(wxCommandEvent& event)
{
	m_settings.linear_filter = event.IsChecked();
	if (m_settings.linear_filter) {
	 	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	 	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	} else {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}
}

void SettingsDialog::onChangeFontBackground(wxCommandEvent& event)
{
	m_settings.visible_label_bg = event.IsChecked();
}

void SettingsDialog::onChangeFontText(wxCommandEvent& event)
{
	m_settings.visible_label_text = event.IsChecked();
}

void SettingsDialog::onChangeNodeName(wxCommandEvent& event)
{
	m_settings.visible_node_name = event.IsChecked();
}

void SettingsDialog::onChangeShape(wxCommandEvent& event)
{
	m_settings.visible_shape = event.IsChecked();
}

void SettingsDialog::onChangeTextureEdge(wxCommandEvent& event)
{
	m_settings.visible_tex_edge = event.IsChecked();
}

void SettingsDialog::onSetBGColor(wxCommandEvent& event)
{
	d2d::RGBColorSettingDlg dlg(this, NULL, m_settings.bg_color);
	if (dlg.ShowModal()) {
		m_settings.bg_color = dlg.GetColor();
	}
}

}