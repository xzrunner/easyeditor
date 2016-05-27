#include "EditPolylinesCMPT.h"
#include "EditPolylinesOP.h"
#include "ChainShape.h"
#include "Math.h"

#include <ee/EditPanel.h>
#include <ee/MultiShapesImpl.h>
#include <ee/ShapeSelection.h>
#include <ee/CountVerifyVisitor.h>
#include <ee/FetchAllVisitor.h>
#include <ee/shape_msg.h>
#include <ee/panel_msg.h>

#include <wx/spinctrl.h>

namespace eshape
{

EditPolylinesCMPT::EditPolylinesCMPT(wxWindow* parent, const std::string& name,
									 ee::EditPanel* editPanel, ee::MultiShapesImpl* shapes_impl)
	: ee::EditCMPT(parent, name, editPanel->GetStageImpl())
	, m_shapes_impl(shapes_impl)
	, m_simplify_spin(NULL)
	, m_btn_merge(NULL)
{
	m_editop = new EditPolylinesOP(editPanel, editPanel->GetStageImpl(), shapes_impl, this);
}

void EditPolylinesCMPT::UpdateControlValue()
{
	bool valid;
	m_shapes_impl->GetShapeSelection()->Traverse(ee::CountVerifyVisitor(valid, 2));
	m_btn_merge->Enable(valid);
}

float EditPolylinesCMPT::GetSimplifyThreshold() const 
{
	return m_simplify_spin->GetValue() * 0.1f;
}

wxSizer* EditPolylinesCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(InitSimplifyPanel());
	sizer->AddSpacer(20);
	sizer->Add(InitEditPanel());

	return sizer;
}

wxSizer* EditPolylinesCMPT::InitSimplifyPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("化简"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	m_simplify_spin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxSize(70, -1), wxSP_ARROW_KEYS, 0, 1000, 30);
	Connect(m_simplify_spin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EditPolylinesCMPT::OnSimplifyThresholdChanged));
	sizer->Add(m_simplify_spin);

	wxButton* btnOK = new wxButton(this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxSize(70, -1));
	Connect(btnOK->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::OnUpdateFromSimplified));
	sizer->Add(btnOK);

	return sizer;
}

wxSizer* EditPolylinesCMPT::InitSmoothPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("平滑"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	return sizer;
}

wxSizer* EditPolylinesCMPT::InitEditPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT(""));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		m_btn_merge = new wxButton(this, wxID_ANY, wxT("合并"));
		m_btn_merge->Enable(false);
		Connect(m_btn_merge->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::OnMergeTwoChain));
		sizer->Add(m_btn_merge);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("水平偏移到(x=0)"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::OnTranslate));
		sizer->Add(btn);
	}
	return sizer;
}

void EditPolylinesCMPT::OnSimplifyThresholdChanged(wxSpinEvent& event)
{
	static_cast<EditPolylinesOP*>(m_editop)->simplify();
}

void EditPolylinesCMPT::OnUpdateFromSimplified(wxCommandEvent& event)
{
	static_cast<EditPolylinesOP*>(m_editop)->updateFromSimplified();
}

void EditPolylinesCMPT::OnMergeTwoChain(wxCommandEvent& event)
{
	std::vector<ee::Shape*> shapes;
	m_shapes_impl->GetShapeSelection()->Traverse(ee::FetchAllVisitor<ee::Shape>(shapes));
	if (shapes.size() == 2)
	{
		ChainShape *chain0 = static_cast<ChainShape*>(shapes[0]),
			       *chain1 = static_cast<ChainShape*>(shapes[1]);

		std::vector<sm::vec2> merged;
		Math::mergeTwoChains(*chain0, *chain1, merged);

		chain0->SetVertices(merged);
		ee::RemoveShapeSJ::Instance()->Remove(chain1);
		m_shapes_impl->GetShapeSelection()->Clear();

		m_shapes_impl->GetShapeSelection()->Clear();

		m_btn_merge->Enable(false);

		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void EditPolylinesCMPT::OnTranslate(wxCommandEvent& event)
{
	std::vector<ee::Shape*> shapes;
	m_shapes_impl->GetShapeSelection()->Traverse(ee::FetchAllVisitor<ee::Shape>(shapes));

	float leftmost = FLT_MAX;
	std::vector<PolylineShape*> polylines;
	for (int i = 0, n = shapes.size(); i < n; ++i)
	{
		PolylineShape* polyline = dynamic_cast<PolylineShape*>(shapes[i]);
		if (polyline) 
		{
			polylines.push_back(polyline);
			if (polyline->GetRect().xmin < leftmost)
				leftmost = polyline->GetRect().xmin;
		}
	}

	const sm::vec2 offset(-leftmost, 0.0f);
	for (size_t i = 0, n = polylines.size(); i < n; ++i) {
		polylines[i]->Translate(offset);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}