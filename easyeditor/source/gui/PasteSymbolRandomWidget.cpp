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
	std::vector<Symbol*> symbols;
	m_library->Traverse(FilterSymbolVisitor(m_sym_filter_ctrl->GetValue().ToStdString(), symbols));
	if (symbols.empty()) {
		val.symbol = NULL;
	} else {
		val.symbol = symbols[static_cast<int>(symbols.size() * Random::GetNum0To1())];
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
FilterSymbolVisitor(const std::string& filter, std::vector<Symbol*>& result)
	: m_filter(filter)
	, m_result(result)
{
}

void PasteSymbolRandomWidget::FilterSymbolVisitor::
Visit(Object* object, bool& next)
{
	Symbol* symbol = static_cast<Symbol*>(object);
	if (!m_filter.empty() && symbol->GetFilepath().find(m_filter) != wxNOT_FOUND)
		m_result.push_back(symbol);
	next = true;
}

}