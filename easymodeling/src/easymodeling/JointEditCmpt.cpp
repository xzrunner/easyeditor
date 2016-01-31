
#include "JointEditCmpt.h"
#include "StagePanel.h"
#include "SelectJointOP.h"

#include <easymodeling.h>

using namespace emodeling;

JointEditCmpt::JointEditCmpt(wxWindow* parent, const wxString& name, 
							 StagePanel* editPanel, ee::PropertySettingPanel* propertyPanel)
	: ee::EditCMPT(parent, name, editPanel->GetStageImpl())
	, m_stage_panel(editPanel)
{
	ee::ArrangeSpriteConfig cfg;
	cfg.is_auto_align_open = false;
	cfg.is_deform_open = false;
	cfg.is_offset_open = false;
	cfg.is_rotate_open = false;
	m_editOP = new ee::ArrangeSpriteOP<SelectJointOP>(editPanel, editPanel->GetStageImpl(), editPanel, propertyPanel, this, cfg);
	static_cast<SelectJointOP*>(m_editOP)->SetPropertyPanel(propertyPanel);
}

void JointEditCmpt::updateControlValue()
{
	ee::SpriteSelection* selection = m_stage_panel->GetSpriteSelection();

	if (selection->Size() != 2)
		m_btnOK->Enable(false);
	else
	{
		if (m_typeChoice->GetString(m_typeChoice->GetSelection()) == wxT("Gear"))
		{
			SelectJointOP* op = static_cast<SelectJointOP*>(m_editOP);
			if (op->jointSelection.Size() == 2)
			{
				std::vector<libmodeling::Joint*> joints;
				op->jointSelection.Traverse(ee::FetchAllVisitor<libmodeling::Joint>(joints));
				const libmodeling::Joint *j0 = joints[0], *j1 = joints[1];
				if ((j0->type == libmodeling::Joint::e_revoluteJoint || j0->type == libmodeling::Joint::e_prismaticJoint) &&
					(j1->type == libmodeling::Joint::e_revoluteJoint || j1->type == libmodeling::Joint::e_prismaticJoint))
					m_btnOK->Enable(true);
				else
					m_btnOK->Enable(false);
			}
			else
				m_btnOK->Enable(false);
		}
		else
			m_btnOK->Enable(true);
	}
}

wxSizer* JointEditCmpt::InitLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Type"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			const int SIZE = 10;

			wxString choices[SIZE];
			choices[0] = wxT("Revolute");
			choices[1] = wxT("Prismatic");
			choices[2] = wxT("Distance");
			choices[3] = wxT("Pulley");
			choices[4] = wxT("Gear");
			choices[5] = wxT("Wheel");
			choices[6] = wxT("Weld");
			choices[7] = wxT("Friction");
			choices[8] = wxT("Rope");
			choices[9] = wxT("Motor");

			m_typeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, SIZE, choices);
			Connect(m_typeChoice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
				wxCommandEventHandler(JointEditCmpt::onTypeChanged));
			m_typeChoice->SetSelection(0);
			sizer->Add(m_typeChoice);
		}
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		m_btnOK = new wxButton(this, wxID_ANY, wxT("OK"));
		Connect(m_btnOK->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(JointEditCmpt::onCreateJoint));
		m_btnOK->Enable(false);
		topSizer->Add(m_btnOK);
	}
	return topSizer;
}

void JointEditCmpt::onCreateJoint(wxCommandEvent& event)
{
	std::vector<ee::Sprite*> sprites;
	ee::SpriteSelection* selection = m_stage_panel->GetSpriteSelection();
	selection->Traverse(ee::FetchAllVisitor<ee::Sprite>(sprites));
	assert(sprites.size() == 2);
	libmodeling::Body *body0 = static_cast<libmodeling::Body*>(sprites[0]->GetUserData()),
		*body1 = static_cast<libmodeling::Body*>(sprites[1]->GetUserData());

	libmodeling::Joint* joint = NULL;
	wxString type = m_typeChoice->GetString(m_typeChoice->GetSelection());
	if (type == wxT("Revolute"))
		m_stage_panel->insertJoint(new libmodeling::RevoluteJoint(body0, body1));
	else if (type == wxT("Prismatic"))
		m_stage_panel->insertJoint(new libmodeling::PrismaticJoint(body0, body1));
	else if (type == wxT("Distance"))
		m_stage_panel->insertJoint(new libmodeling::DistanceJoint(body0, body1));
	else if (type == wxT("Pulley"))
		m_stage_panel->insertJoint(new libmodeling::PulleyJoint(body0, body1));
	else if (type == wxT("Gear"))
	{
		SelectJointOP* op = static_cast<SelectJointOP*>(m_editOP);
		assert(op->jointSelection.Size() == 2);
		std::vector<libmodeling::Joint*> joints;
		op->jointSelection.Traverse(ee::FetchAllVisitor<libmodeling::Joint>(joints));
		m_stage_panel->insertJoint(new libmodeling::GearJoint(body0, body1, joints[0], joints[1]));
	}	
	else if (type == wxT("Wheel"))
	{
		SelectWheelDialog dlg(this, wxT("Choose Wheel"), body0->name, body1->name);
		if (dlg.ShowModal() == wxID_OK)
		{
			if (dlg.getChoice() == 0)
				m_stage_panel->insertJoint(new libmodeling::WheelJoint(body1, body0));
			else
				m_stage_panel->insertJoint(new libmodeling::WheelJoint(body0, body1));
		}
	}
	else if (type == wxT("Weld"))
		m_stage_panel->insertJoint(new libmodeling::WeldJoint(body0, body1));
	else if (type == wxT("Friction"))
		m_stage_panel->insertJoint(new libmodeling::FrictionJoint(body0, body1));
	else if (type == wxT("Rope"))
		m_stage_panel->insertJoint(new libmodeling::RopeJoint(body0, body1));
	else if (type == wxT("Motor"))
		m_stage_panel->insertJoint(new libmodeling::MotorJoint(body0, body1));

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

void JointEditCmpt::onTypeChanged(wxCommandEvent& event)
{
	updateControlValue();
}

//////////////////////////////////////////////////////////////////////////
// class JointEditCmpt::SelectWheelDialog
//////////////////////////////////////////////////////////////////////////

JointEditCmpt::SelectWheelDialog::
SelectWheelDialog(wxWindow* parent, const wxString& title, const wxString& body0, const wxString& body1)
	: wxDialog(parent, wxID_ANY, title)
{
	InitLayout(body0, body1);
}

void JointEditCmpt::SelectWheelDialog::
InitLayout(const wxString& body0, const wxString& body1)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);	
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("wheel"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxString choices[2];
			choices[0] = body0;
			choices[1] = body1;

			m_wheelChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choices);
			m_wheelChoice->SetSelection(0);
			sizer->Add(m_wheelChoice);
		}
		topSizer->Add(sizer, 0, wxALIGN_CENTER_HORIZONTAL);
	}
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
		btnSizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
		btnSizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
		topSizer->Add(btnSizer, 0, wxALL, 5);
	}
	SetSizer(topSizer);
	topSizer->Fit(this);
}
