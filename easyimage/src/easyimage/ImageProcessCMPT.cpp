#include "ImageProcessCMPT.h"
#include "ImageProcessOP.h"
#include "StagePanel.h"

#include <easyimage.h>

#include <ee/ShaderMgr.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/panel_msg.h>

namespace eimage
{

ImageProcessCMPT::ImageProcessCMPT(wxWindow* parent, const std::string& name, 
								   StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_edge_shader(NULL)
{
//	m_editop = new ImageProcessOP(stage);

	m_editop = new ee::ZoomViewOP(stage, stage->GetStageImpl(), true);
}

wxSizer* ImageProcessCMPT::InitLayout()
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
	ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
	ee::SpriteShader* shader = new ee::SpriteShader;
	try {
		shader->Load();
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
//	shader_mgr->SetSpriteShader(shader);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageProcessCMPT::OnEdgeDetection(wxCommandEvent& event)
{
	ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
	m_edge_shader = new EdgeDetectionShader;
	try {
		m_edge_shader->Load();
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
//	shader_mgr->SetSpriteShader(m_edge_shader);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageProcessCMPT::OnSetEdgeBlend(wxScrollEvent& event)
{
	if (m_edge_shader) {
		ee::ShaderMgr::Instance()->sprite();
		m_edge_shader->SetBlend(event.GetInt() * 0.01f);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void ImageProcessCMPT::OnRelief(wxCommandEvent& event)
{
	ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
	ReliefShader* shader = new ReliefShader;
	try {
		shader->Load();
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
//	shader_mgr->SetSpriteShader(shader);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageProcessCMPT::OnOutline(wxCommandEvent& event)
{
	ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
	OutlineShader* shader = new OutlineShader;
	try {
		shader->Load();
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
//	shader_mgr->SetSpriteShader(shader);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageProcessCMPT::OnBlur(wxCommandEvent& event)
{
	ee::ShaderMgr* shader_mgr = ee::ShaderMgr::Instance();
	m_blur_shader = new BlurShader;
	try {
		m_blur_shader->Load();
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
//	shader_mgr->SetSpriteShader(m_blur_shader);
	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void ImageProcessCMPT::OnSetBlurRadius(wxScrollEvent& event)
{
	if (m_blur_shader) {
		ee::ShaderMgr::Instance()->sprite();
		m_blur_shader->SetRadius(event.GetInt() * 0.1f);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

}