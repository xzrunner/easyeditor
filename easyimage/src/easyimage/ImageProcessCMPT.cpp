#include "ImageProcessCMPT.h"
#include "ImageProcessOP.h"
#include "StagePanel.h"

#include <easyimage.h>

namespace eimage
{

ImageProcessCMPT::ImageProcessCMPT(wxWindow* parent, const wxString& name, 
								   StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_edge_shader(NULL)
{
//	m_editOP = new ImageProcessOP(stage);

	m_editOP = new d2d::ZoomViewOP(stage, true);
}

wxSizer* ImageProcessCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "Õý³£");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnNormal));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "±ßÔµ¼ì²â");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnEdgeDetection));
		sizer->Add(btn);

		wxSlider* slider = new wxSlider(this, wxID_ANY, 50, 0, 100);
		Connect(slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(ImageProcessCMPT::OnSetEdgeBlend));
		sizer->Add(slider);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "¸¡µñ");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnRelief));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "°üÎ§");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnOutline));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "Ä£ºý");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ImageProcessCMPT::OnBlur));
		sizer->Add(btn);

		wxSlider* slider = new wxSlider(this, wxID_ANY, 10, 10, 100);
		Connect(slider->GetId(), wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(ImageProcessCMPT::OnSetBlurRadius));
		sizer->Add(slider);
	}
	return sizer;
}

void ImageProcessCMPT::OnNormal(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	d2d::SpriteShader* shader = new d2d::SpriteShader;
	try {
		shader->Load();
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
	shader_mgr->SetSpriteShader(shader);
	m_stage->GetCanvas()->ResetViewport();
	m_stage->GetCanvas()->SetDirty();
}

void ImageProcessCMPT::OnEdgeDetection(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	m_edge_shader = new EdgeDetectionShader;
	try {
		m_edge_shader->Load();
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
	shader_mgr->SetSpriteShader(m_edge_shader);
	m_stage->GetCanvas()->ResetViewport();
	m_stage->GetCanvas()->SetDirty();
}

void ImageProcessCMPT::OnSetEdgeBlend(wxScrollEvent& event)
{
	if (m_edge_shader) {
		d2d::ShaderMgr::Instance()->sprite();
		m_edge_shader->SetBlend(event.GetInt() * 0.01f);
		m_stage->GetCanvas()->SetDirty();
	}
}

void ImageProcessCMPT::OnRelief(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	ReliefShader* shader = new ReliefShader;
	try {
		shader->Load();
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
	shader_mgr->SetSpriteShader(shader);
	m_stage->GetCanvas()->ResetViewport();
	m_stage->GetCanvas()->SetDirty();
}

void ImageProcessCMPT::OnOutline(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	OutlineShader* shader = new OutlineShader;
	try {
		shader->Load();
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
	shader_mgr->SetSpriteShader(shader);
	m_stage->GetCanvas()->ResetViewport();
	m_stage->GetCanvas()->SetDirty();
}

void ImageProcessCMPT::OnBlur(wxCommandEvent& event)
{
	d2d::ShaderMgr* shader_mgr = d2d::ShaderMgr::Instance();
	m_blur_shader = new BlurShader;
	try {
		m_blur_shader->Load();
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
	shader_mgr->SetSpriteShader(m_blur_shader);
	m_stage->GetCanvas()->ResetViewport();
	m_stage->GetCanvas()->SetDirty();
}

void ImageProcessCMPT::OnSetBlurRadius(wxScrollEvent& event)
{
	if (m_blur_shader) {
		d2d::ShaderMgr::Instance()->sprite();
		m_blur_shader->SetRadius(event.GetInt() * 0.1f);
		m_stage->GetCanvas()->SetDirty();
	}
}

}