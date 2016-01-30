#include "EditPolylinesCMPT.h"
#include "EditPolylinesOP.h"
#include "ChainShape.h"
#include "Math.h"

namespace libshape
{

EditPolylinesCMPT::EditPolylinesCMPT(wxWindow* parent, const wxString& name,
									 d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl)
	: d2d::AbstractEditCMPT(parent, name, editPanel->GetStageImpl())
	, m_shapesImpl(shapesImpl)
	, m_simplifySpin(NULL)
	, m_btnMerge(NULL)
{
	m_editOP = new EditPolylinesOP(editPanel, editPanel->GetStageImpl(), shapesImpl, this);
}

void EditPolylinesCMPT::updateControlValue()
{
	bool valid;
	m_shapesImpl->GetShapeSelection()->Traverse(d2d::CountVerifyVisitor(valid, 2));
	m_btnMerge->Enable(valid);
}

float EditPolylinesCMPT::getSimplifyThreshold() const 
{
	return m_simplifySpin->GetValue() * 0.1f;
}

wxSizer* EditPolylinesCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(initSimplifyPanel());
	sizer->AddSpacer(20);
	sizer->Add(initEditPanel());

	return sizer;
}

wxSizer* EditPolylinesCMPT::initSimplifyPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("化简"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	m_simplifySpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxSize(70, -1), wxSP_ARROW_KEYS, 0, 1000, 30);
	Connect(m_simplifySpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EditPolylinesCMPT::onSimplifyThresholdChanged));
	sizer->Add(m_simplifySpin);

	wxButton* btnOK = new wxButton(this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxSize(70, -1));
	Connect(btnOK->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::onUpdateFromSimplified));
	sizer->Add(btnOK);

	return sizer;
}

wxSizer* EditPolylinesCMPT::initSmoothPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("平滑"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	return sizer;
}

wxSizer* EditPolylinesCMPT::initEditPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT(""));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		m_btnMerge = new wxButton(this, wxID_ANY, wxT("合并"));
		m_btnMerge->Enable(false);
		Connect(m_btnMerge->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::onMergeTwoChain));
		sizer->Add(m_btnMerge);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("水平偏移到(x=0)"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::onTranslate));
		sizer->Add(btn);
	}
	return sizer;
}

void EditPolylinesCMPT::onSimplifyThresholdChanged(wxSpinEvent& event)
{
	static_cast<EditPolylinesOP*>(m_editOP)->simplify();
}

void EditPolylinesCMPT::onUpdateFromSimplified(wxCommandEvent& event)
{
	static_cast<EditPolylinesOP*>(m_editOP)->updateFromSimplified();
}

void EditPolylinesCMPT::onMergeTwoChain(wxCommandEvent& event)
{
	std::vector<d2d::Shape*> shapes;
	m_shapesImpl->GetShapeSelection()->Traverse(d2d::FetchAllVisitor<d2d::Shape>(shapes));
	if (shapes.size() == 2)
	{
		ChainShape *chain0 = static_cast<ChainShape*>(shapes[0]),
			*chain1 = static_cast<ChainShape*>(shapes[1]);

		std::vector<d2d::Vector> merged;
		Math::mergeTwoChains(*chain0, *chain1, merged);

		chain0->Load(merged);
		chain0->refresh();
		d2d::RemoveShapeSJ::Instance()->Remove(chain1);
		m_shapesImpl->GetShapeSelection()->Clear();

		m_shapesImpl->GetShapeSelection()->Clear();

		m_btnMerge->Enable(false);

		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void EditPolylinesCMPT::onTranslate(wxCommandEvent& event)
{
	std::vector<d2d::Shape*> shapes;
	m_shapesImpl->GetShapeSelection()->Traverse(d2d::FetchAllVisitor<d2d::Shape>(shapes));

	float leftmost = FLT_MAX;
	std::vector<ChainShape*> chains;
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
	{
		ChainShape* chain = dynamic_cast<ChainShape*>(shapes[i]);
		if (chain) 
		{
			chains.push_back(chain);
			if (chain->GetRect().xmin < leftmost)
				leftmost = chain->GetRect().xmin;
		}
	}

	const d2d::Vector offset(-leftmost, 0.0f);
	for (size_t i = 0, n = chains.size(); i < n; ++i) {
		chains[i]->Translate(offset);
	}

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

}