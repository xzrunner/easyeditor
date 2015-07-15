#include "AutoRectCutCMPT.h"
#include "AutoRectCutOP.h"
#include "StagePanel.h"

#include <easyimage.h>

namespace eimage
{

AutoRectCutCMPT::AutoRectCutCMPT(wxWindow* parent, const wxString& name, 
								 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_editOP = new AutoRectCutOP(stage);
}

wxSizer* AutoRectCutCMPT::initLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

//	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Add Rect"));
//	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
// 	{
// 		wxSizer* input_sizer = new wxBoxSizer(wxVERTICAL);
// 
// 		wxString choices[5] = {"8", "16", "32", "64", "128"};
// 
// 		wxSizer* width_sizer = new wxBoxSizer(wxHORIZONTAL);
// 		width_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Width: ")));
// 		m_width_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choices);
// 		m_width_choice->SetSelection(3);
// 		width_sizer->Add(m_width_choice);
// 		input_sizer->Add(width_sizer);
// 
// 		wxSizer* height_sizer = new wxBoxSizer(wxHORIZONTAL);
// 		height_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Height: ")));
// 		m_height_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choices);
// 		m_height_choice->SetSelection(3);
// 		height_sizer->Add(m_height_choice);
// 		input_sizer->Add(height_sizer);
// 
// 		sizer->Add(input_sizer);
// 	}
// 	sizer->AddSpacer(10);
// 	{
// 		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Add"));
// 		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
// 			wxCommandEventHandler(AutoRectCutCMPT::OnAddRect));
// 		sizer->Add(btn);
// 	}
//	sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Auto"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoRectCutCMPT::OnCreateRects));
		top_sizer->Add(btn);
	}
	top_sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoRectCutCMPT::OnOutputRects));
		top_sizer->Add(btn);		
	}

	return top_sizer;
}

// void AutoRectCutCMPT::OnAddRect(wxCommandEvent& event)
// {
// 	int width = wxVariant(m_width_choice->GetString(m_width_choice->GetSelection())).GetInteger();
// 	int height = wxVariant(m_height_choice->GetString(m_height_choice->GetSelection())).GetInteger();
// 
// 	AutoRectCutOP* op = static_cast<AutoRectCutOP*>(m_editOP);
// 	op->getRectMgr().insert(d2d::Rect(d2d::Vector(0, 0), d2d::Vector((float)width, (float)height)));
// 
// 	m_stage->Refresh();
// }

void AutoRectCutCMPT::OnCreateRects(wxCommandEvent& event)
{
	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->GetSymbol().getImage();

	RegularRectCut cut(*img);
	cut.AutoCut();

	const std::vector<Rect>& result = cut.GetResult();
	RectMgr& rects = static_cast<AutoRectCutOP*>(m_editOP)->getRectMgr();
	for (int i = 0, n = result.size(); i < n; ++i) {
		int x = result[i].x,
			y = result[i].y,
			w = result[i].w,
			h = result[i].h;
		rects.insert(d2d::Rect(d2d::Vector(x, y), d2d::Vector(x+w, y+h)), true);
	}

	m_stage->SetCanvasDirty();

	wxString msg;
	msg.Printf("Left: %d, Used: %d", cut.GetLeftArea(), cut.GetUseArea());
	wxMessageBox(msg, wxT("Statics"), wxOK | wxICON_INFORMATION, this);
}

void AutoRectCutCMPT::OnOutputRects(wxCommandEvent& event)
{
	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->GetSymbol().getImage();

	RegularRectCut cut(*img);
	cut.AutoCut();

	wxString msg;
	msg.Printf("Left: %d, Used: %d", cut.GetLeftArea(), cut.GetUseArea());
	wxMessageBox(msg, wxT("Statics"), wxOK | wxICON_INFORMATION, this);

	wxString ori_path = d2d::FilenameTools::getFilePathExceptExtension(img->GetFilepath());
	eimage::ImageClip img_cut(*img->GetImageData());
	const std::vector<Rect>& result = cut.GetResult();
	for (int i = 0, n = result.size(); i < n; ++i) {
		const eimage::Rect& r = result[i];
		const uint8_t* pixels = img_cut.Clip(r.x, r.x+r.w, r.y, r.y+r.h);

		wxString out_path;
		out_path.Printf("%s#%d#%d#%d#%d#", ori_path, r.x, r.y, r.w, r.h);
		d2d::ImageSaver::storeToFile(pixels, r.w, r.h, 4, out_path.ToStdString(), d2d::ImageSaver::e_png);
		delete[] pixels;
	}
}

}