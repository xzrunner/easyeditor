#include "AutoCutCMPT.h"
#include "AutoCutOP.h"
#include "StagePanel.h"

#include <easyimage.h>

namespace eimage
{

static const float TOLERANCE = 0.04f;

AutoCutCMPT::AutoCutCMPT(wxWindow* parent, const wxString& name, 
						 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
	, m_raw(NULL)
	, m_fine(NULL)
{
	m_editOP = new AutoCutOP(stage);
}

wxSizer* AutoCutCMPT::initLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Trigger"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoCutCMPT::Trigger));
		top_sizer->Add(btn);
	}
	top_sizer->AddSpacer(20);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("step"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Create Outline"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(AutoCutCMPT::CreateOutline));
			sizer->Add(btn);
		}
		sizer->AddSpacer(5);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Reduce Count"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(AutoCutCMPT::ReduceOutlineCount));
			sizer->Add(btn);
		}
		top_sizer->Add(sizer);
	}
	return top_sizer;
}

void AutoCutCMPT::Trigger(wxCommandEvent& event)
{ 
// 	// step by step
// 	static int max_count = 5;

	const d2d::ISprite* sprite = m_stage->getImage();
 	const d2d::ImageSprite* img_sprite 
 		= dynamic_cast<const d2d::ImageSprite*>(sprite);
 	assert(img_sprite);
 	const d2d::Image* img = img_sprite->getSymbol().getImage();
 
 	AutoCutOP* op = static_cast<AutoCutOP*>(m_editOP);
 	ExtractOutlineRaw raw(*img);
 	raw.Trigger();
 	op->m_raw_bound_line = raw.GetBorderLine();
 	op->m_raw_bound_points = raw.GetBorderPoints();
 	op->m_raw_bound_line_merged = raw.GetBorderLineMerged();

 	ExtractOutlineFine fine(op->m_raw_bound_line, op->m_raw_bound_line_merged);
 	fine.Trigger(TOLERANCE);
//	fine.Trigger(TOLERANCE, max_count++);
 	op->m_fine_bound_line = fine.GetResult();
 
 	m_editPanel->Refresh();
}

void AutoCutCMPT::CreateOutline(wxCommandEvent& event)
{
	// step by step
	static int max_count = 5;

	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->getSymbol().getImage();

	AutoCutOP* op = static_cast<AutoCutOP*>(m_editOP);
	m_raw = new ExtractOutlineRaw(*img);
	m_raw->Trigger();
	op->m_raw_bound_line = m_raw->GetBorderLine();
	op->m_raw_bound_points = m_raw->GetBorderPoints();
	op->m_raw_bound_line_merged = m_raw->GetBorderLineMerged();

	m_fine = new ExtractOutlineFine(op->m_raw_bound_line, op->m_raw_bound_line_merged);
	m_fine->CreateOutline(TOLERANCE, max_count++);
	op->m_fine_bound_line = m_fine->GetResult();

	m_editPanel->Refresh();
}

void AutoCutCMPT::ReduceOutlineCount(wxCommandEvent& event)
{
	if (m_fine)
	{
		m_fine->ReduceOutlineCount(TOLERANCE);
		AutoCutOP* op = static_cast<AutoCutOP*>(m_editOP);
		op->m_fine_bound_line = m_fine->GetResult();
		m_editPanel->Refresh();
	}
}

}