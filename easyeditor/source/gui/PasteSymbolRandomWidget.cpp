#include "PasteSymbolRandomWidget.h"
#include "Random.h"
#include "Math2D.h"
#include "Symbol.h"
#include "LibraryPanel.h"

#include <wx/spinctrl.h>

namespace ee
{

PasteSymbolRandomWidget::PasteSymbolRandomWidget(wxWindow* parent, LibraryPanel* library)
	: wxPanel(parent)
	, m_library(library)
{
	InitLayout();
}

void PasteSymbolRandomWidget::InitLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("随机参数"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	topSizer->AddSpacer(5);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("元件过滤"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		sizer->Add(m_sym_filter_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(5);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Scale"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		m_scale_min_ctrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 100, 75);
		sizer->Add(m_scale_min_ctrl);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT(" - ")));
		m_scale_max_ctrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 100, 100);
		sizer->Add(m_scale_max_ctrl);
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(5);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Angle"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		m_angle_min_ctrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(55, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, -180, 180, -45);
		sizer->Add(m_angle_min_ctrl);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT(" - ")));
		m_angle_max_ctrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(55, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, -180, 180, 45);
		sizer->Add(m_angle_max_ctrl);
		topSizer->Add(sizer);
	}
	SetSizer(topSizer);
}

void PasteSymbolRandomWidget::GetRandomValue(RandomValue& val) const
{
	FilterSymbolVisitor visitor(m_sym_filter_ctrl->GetValue().ToStdString().c_str());
	m_library->Traverse(visitor);
	auto syms = visitor.GetResult();
	if (syms.empty()) {
		val.sym = NULL;
	} else {
		val.sym = syms[static_cast<int>(syms.size() * Random::GetNum0To1())];
	}

	const float scaleMin = m_scale_min_ctrl->GetValue() * 0.01f,
		scaleMax = m_scale_max_ctrl->GetValue() * 0.01f;
	val.scale = scaleMin + (scaleMax - scaleMin) * Random::GetNum0To1();

	const float angleMin = m_angle_min_ctrl->GetValue() * SM_DEG_TO_RAD,
		angleMax = m_angle_max_ctrl->GetValue() * SM_DEG_TO_RAD;
	val.angle = angleMin + (angleMax - angleMin) * Random::GetNum0To1();
}

//////////////////////////////////////////////////////////////////////////
// class PasteSymbolRandomWidget::FilterSymbolVisitor
//////////////////////////////////////////////////////////////////////////

PasteSymbolRandomWidget::FilterSymbolVisitor::
FilterSymbolVisitor(const std::string& filter)
	: m_filter(filter)
{
}

void PasteSymbolRandomWidget::FilterSymbolVisitor::
Visit(const ListItemPtr& item, bool& next)
{
	auto sym = std::static_pointer_cast<ee::Symbol>(item);
	if (!m_filter.empty() && sym->GetFilepath().find(m_filter.c_str()) != wxNOT_FOUND)
		m_result.push_back(sym);
	next = true;
}

}