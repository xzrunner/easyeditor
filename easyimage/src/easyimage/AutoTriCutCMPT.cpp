#include "AutoTriCutCMPT.h"
#include "AutoTriCutOP.h"
#include "StagePanel.h"

#include <easyimage.h>

namespace eimage
{

static const float AREA_TOLERANCE = 0.04f;
static const float PERIMETER_TOLERANCE = 0.2f;

#define TRIGGER_STEP		// step by step

AutoTriCutCMPT::AutoTriCutCMPT(wxWindow* parent, const wxString& name, 
						 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
	, m_raw(NULL)
	, m_fine(NULL)
{
	m_editOP = new AutoTriCutOP(stage);
}

wxSizer* AutoTriCutCMPT::initLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Trigger"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoTriCutCMPT::Trigger));
		top_sizer->Add(btn);
	}
	top_sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output Outline"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoTriCutCMPT::OutputOutline));
		top_sizer->Add(btn);
	}
	top_sizer->AddSpacer(20);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("step"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Create Outline"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(AutoTriCutCMPT::CreateOutline));
			sizer->Add(btn);
		}
		sizer->AddSpacer(5);
		{
			wxButton* btn = new wxButton(this, wxID_ANY, wxT("Reduce Count"));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
				wxCommandEventHandler(AutoTriCutCMPT::ReduceOutlineCount));
			sizer->Add(btn);
		}
		top_sizer->Add(sizer);
	}
	top_sizer->AddSpacer(20);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("debug"));
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Trigger"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(AutoTriCutCMPT::OnDebug));
		sizer->Add(btn);
		top_sizer->Add(sizer);
	}
	return top_sizer;
}

void AutoTriCutCMPT::Trigger(wxCommandEvent& event)
{
	Trigger();
}

void AutoTriCutCMPT::OutputOutline(wxCommandEvent& event)
{
	Trigger();

	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->GetSymbol().getImage();

	Json::Value value;
	AutoTriCutOP* op = static_cast<AutoTriCutOP*>(m_editOP);

	d2d::Vector offset(-0.5f*img->GetOriginWidth(), -0.5f*img->GetOriginHeight());
	std::vector<d2d::Vector> vertices(op->m_fine_bound_line);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		vertices[i] += offset;
	}
	d2d::JsonIO::Store(vertices, value["normal"]);

	wxString filepath = d2d::FilenameTools::getFilenameAddTag(img->GetFilepath(), 
		OUTLINE_FILE_TAG, "json");
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.fn_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void AutoTriCutCMPT::CreateOutline(wxCommandEvent& event)
{
	// step by step
	static int max_step = 5;

	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->GetSymbol().getImage();

	AutoTriCutOP* op = static_cast<AutoTriCutOP*>(m_editOP);
	m_raw = new ExtractOutlineRaw(*img);
	m_raw->CreateBorderLineAndMerge();
	op->m_raw_bound_line = m_raw->GetBorderLine();
	op->m_raw_bound_points = m_raw->GetBorderPoints();
	op->m_raw_bound_line_merged = m_raw->GetBorderLineMerged();

	m_fine = new ExtractOutlineFine(op->m_raw_bound_line, op->m_raw_bound_line_merged);
	m_fine->CreateOutline(AREA_TOLERANCE, PERIMETER_TOLERANCE, max_step++);
	op->m_fine_bound_line = m_fine->GetResult();

	m_stage->GetCanvas()->SetDirty();
}

void AutoTriCutCMPT::ReduceOutlineCount(wxCommandEvent& event)
{
	if (m_fine)
	{
		m_fine->ReduceOutlineCount(AREA_TOLERANCE, PERIMETER_TOLERANCE);
		AutoTriCutOP* op = static_cast<AutoTriCutOP*>(m_editOP);
		op->m_fine_bound_line = m_fine->GetResult();
		m_stage->GetCanvas()->SetDirty();
	}
}

void AutoTriCutCMPT::Trigger()
{
#ifdef TRIGGER_STEP
	static int max_step = 5;
#endif
	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->GetSymbol().getImage();

	AutoTriCutOP* op = static_cast<AutoTriCutOP*>(m_editOP);
	ExtractOutlineRaw raw(*img);
	raw.CreateBorderLineAndMerge();
	op->m_raw_bound_line = raw.GetBorderLine();
	op->m_raw_bound_points = raw.GetBorderPoints();
	op->m_raw_bound_line_merged = raw.GetBorderLineMerged();

	ExtractOutlineFine fine(op->m_raw_bound_line, op->m_raw_bound_line_merged);
#ifdef TRIGGER_STEP
	fine.Trigger(AREA_TOLERANCE, PERIMETER_TOLERANCE, max_step++);
#else
	fine.Trigger(AREA_TOLERANCE, PERIMETER_TOLERANCE);
#endif
	op->m_fine_bound_line = fine.GetResult();

	m_stage->GetCanvas()->SetDirty();
}

void AutoTriCutCMPT::OnDebug(wxCommandEvent& event)
{
	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->GetSymbol().getImage();

	AutoTriCutOP* op = static_cast<AutoTriCutOP*>(m_editOP);
	ExtractOutlineRaw raw(*img);
	raw.CreateBorderLineAndMerge();
	raw.CreateBorderConvexHull();
	op->m_raw_bound_line = raw.GetConvexHull();
	op->m_raw_bound_points = raw.GetBorderPoints();
	op->m_raw_bound_line_merged = raw.GetBorderLineMerged();
}

}