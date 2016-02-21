#include "JointPropertySetting.h"

#include <ee/panel_msg.h>

namespace emodeling
{

JointPropertySetting::JointPropertySetting(ee::EditPanelImpl* stage, Joint* joint)
	: ee::PropertySetting("Joint")
	, m_stage(stage)
{
	m_joint = joint;

	switch (joint->m_type)
	{
	case Joint::e_revoluteJoint:
		m_type = "Revolute Joint";
		break;
	case Joint::e_prismaticJoint:
		m_type = "Prismatic Joint";
		break;
	case Joint::e_distanceJoint:
		m_type = "Distance Joint";
		break;
	case Joint::e_pulleyJoint:
		m_type = "Pulley Joint";
		break;
	case Joint::e_mouseJoint:
		m_type = "Mouse Joint";
		break;
	case Joint::e_gearJoint:
		m_type = "Gear Joint";
		break;
	case Joint::e_wheelJoint:
		m_type = "Wheel Joint";
		break;
	case Joint::e_weldJoint:
		m_type = "Weld Joint";
		break;
	case Joint::e_frictionJoint:
		m_type = "Friction Joint";
		break;
	case Joint::e_ropeJoint:
		m_type = "Rope Joint";
		break;
	case Joint::e_motorJoint:
		m_type = "Motor Joint";
		break;
	}
}

void JointPropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	bool dirty = true;
	if (name == wxT("Name"))
		m_joint->m_name = wxANY_AS(value, wxString);
	else if (name == wxT("collideConnected"))
		m_joint->m_collide_connected = wxANY_AS(value, bool);
	switch (m_joint->m_type)
	{
	case Joint::e_revoluteJoint:
		OnPropertyGridChange(static_cast<RevoluteJoint*>(m_joint), name, value);
		break;
	case Joint::e_prismaticJoint:
		OnPropertyGridChange(static_cast<PrismaticJoint*>(m_joint), name, value);
		break;
	case Joint::e_distanceJoint:
		OnPropertyGridChange(static_cast<DistanceJoint*>(m_joint), name, value);
		break;
	case Joint::e_pulleyJoint:
		OnPropertyGridChange(static_cast<PulleyJoint*>(m_joint), name, value);
		break;
	case Joint::e_gearJoint:
		OnPropertyGridChange(static_cast<GearJoint*>(m_joint), name, value);
		break;
	case Joint::e_wheelJoint:
		OnPropertyGridChange(static_cast<WheelJoint*>(m_joint), name, value);
		break;
	case Joint::e_weldJoint:
		OnPropertyGridChange(static_cast<WeldJoint*>(m_joint), name, value);
		break;
	case Joint::e_frictionJoint:
		OnPropertyGridChange(static_cast<FrictionJoint*>(m_joint), name, value);
		break;
	case Joint::e_ropeJoint:
		OnPropertyGridChange(static_cast<RopeJoint*>(m_joint), name, value);
		break;
	case Joint::e_motorJoint:
		OnPropertyGridChange(static_cast<MotorJoint*>(m_joint), name, value);
		break;
	}

	if (dirty) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void JointPropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("Name"))->SetValue(m_joint->m_name);
	pg->GetProperty(wxT("collideConnected"))->SetValue(m_joint->m_collide_connected);

	switch (m_joint->m_type)
	{
	case Joint::e_revoluteJoint:
		UpdatePropertyPanel(static_cast<RevoluteJoint*>(m_joint), pg);
		break;
	case Joint::e_prismaticJoint:
		UpdatePropertyPanel(static_cast<PrismaticJoint*>(m_joint), pg);
		break;
	case Joint::e_distanceJoint:
		UpdatePropertyPanel(static_cast<DistanceJoint*>(m_joint), pg);
		break;
	case Joint::e_pulleyJoint:
		UpdatePropertyPanel(static_cast<PulleyJoint*>(m_joint), pg);
		break;
	case Joint::e_gearJoint:
		UpdatePropertyPanel(static_cast<GearJoint*>(m_joint), pg);
		break;
	case Joint::e_wheelJoint:
		UpdatePropertyPanel(static_cast<WheelJoint*>(m_joint), pg);
		break;
	case Joint::e_weldJoint:
		UpdatePropertyPanel(static_cast<WeldJoint*>(m_joint), pg);
		break;
	case Joint::e_frictionJoint:
		UpdatePropertyPanel(static_cast<FrictionJoint*>(m_joint), pg);
		break;
	case Joint::e_ropeJoint:
		UpdatePropertyPanel(static_cast<RopeJoint*>(m_joint), pg);
		break;
	case Joint::e_motorJoint:
		UpdatePropertyPanel(static_cast<MotorJoint*>(m_joint), pg);
		break;
	}
}

void JointPropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_joint->m_name));
	pg->Append(new wxBoolProperty(wxT("collideConnected"), wxPG_LABEL, m_joint->m_collide_connected));
	pg->SetPropertyAttribute("collideConnected", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	switch (m_joint->m_type)
	{
	case Joint::e_revoluteJoint:
		CreatePropertyPanel(static_cast<RevoluteJoint*>(m_joint), pg);
		break;
	case Joint::e_prismaticJoint:
		CreatePropertyPanel(static_cast<PrismaticJoint*>(m_joint), pg);
		break;
	case Joint::e_distanceJoint:
		CreatePropertyPanel(static_cast<DistanceJoint*>(m_joint), pg);
		break;
	case Joint::e_pulleyJoint:
		CreatePropertyPanel(static_cast<PulleyJoint*>(m_joint), pg);
		break;
	case Joint::e_gearJoint:
		CreatePropertyPanel(static_cast<GearJoint*>(m_joint), pg);
		break;
	case Joint::e_wheelJoint:
		CreatePropertyPanel(static_cast<WheelJoint*>(m_joint), pg);
		break;
	case Joint::e_weldJoint:
		CreatePropertyPanel(static_cast<WeldJoint*>(m_joint), pg);
		break;
	case Joint::e_frictionJoint:
		CreatePropertyPanel(static_cast<FrictionJoint*>(m_joint), pg);
		break;
	case Joint::e_ropeJoint:
		CreatePropertyPanel(static_cast<RopeJoint*>(m_joint), pg);
		break;
	case Joint::e_motorJoint:
		CreatePropertyPanel(static_cast<MotorJoint*>(m_joint), pg);
		break;
	}
}

void JointPropertySetting::CreatePropertyPanel(RevoluteJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("referenceAngle"), wxPG_LABEL, joint->m_reference_angle));
	pg->SetPropertyAttribute(wxT("referenceAngle"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableLimit"), wxPG_LABEL, joint->m_enable_limit));
	pg->SetPropertyAttribute("enableLimit", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("lowerAngle"), wxPG_LABEL, joint->m_lower_angle));
	pg->SetPropertyAttribute(wxT("lowerAngle"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("upperAngle"), wxPG_LABEL, joint->m_upper_angle));
	pg->SetPropertyAttribute(wxT("upperAngle"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableMotor"), wxPG_LABEL, joint->m_enable_motor));
	pg->SetPropertyAttribute("enableMotor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("maxMotorTorque"), wxPG_LABEL, joint->m_max_motor_torque));
	pg->SetPropertyAttribute(wxT("maxMotorTorque"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("motorSpeed"), wxPG_LABEL, joint->m_motor_speed));
	pg->SetPropertyAttribute(wxT("motorSpeed"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(RevoluteJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("referenceAngle"))->SetValue(joint->m_reference_angle);

	pg->GetProperty(wxT("enableLimit"))->SetValue(joint->m_enable_limit);

	pg->GetProperty(wxT("lowerAngle"))->SetValue(joint->m_lower_angle);
	pg->GetProperty(wxT("upperAngle"))->SetValue(joint->m_upper_angle);

	pg->GetProperty(wxT("enableMotor"))->SetValue(joint->m_enable_motor);

	pg->GetProperty(wxT("maxMotorTorque"))->SetValue(joint->m_max_motor_torque);

	pg->GetProperty(wxT("motorSpeed"))->SetValue(joint->m_motor_speed);
}

void JointPropertySetting::OnPropertyGridChange(RevoluteJoint* joint, 
												const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("referenceAngle"))
		joint->m_reference_angle = wxANY_AS(value, float);
	else if (name == wxT("enableLimit"))
		joint->m_enable_limit = wxANY_AS(value, bool);
	else if (name == wxT("lowerAngle"))
		joint->m_lower_angle = wxANY_AS(value, float);
	else if (name == wxT("upperAngle"))
		joint->m_upper_angle = wxANY_AS(value, float);
	else if (name == wxT("enableMotor"))
		joint->m_enable_motor = wxANY_AS(value, bool);
	else if (name == wxT("maxMotorTorque"))
		joint->m_max_motor_torque = wxANY_AS(value, float);
	else if (name == wxT("motorSpeed"))
		joint->m_motor_speed = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(PrismaticJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	wxPGProperty* localAxisAProp = pg->Append(new wxStringProperty(wxT("localAxisA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_axis_a.x));
	pg->SetPropertyAttribute(wxT("localAxisA.x"), "Precision", 2);
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_axis_a.y));
	pg->SetPropertyAttribute(wxT("localAxisA.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("referenceAngle"), wxPG_LABEL, joint->m_reference_angle));
	pg->SetPropertyAttribute(wxT("referenceAngle"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableLimit"), wxPG_LABEL, joint->m_enable_limit));
	pg->SetPropertyAttribute("enableLimit", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("lowerTranslation"), wxPG_LABEL, joint->m_lower_translation));
	pg->SetPropertyAttribute(wxT("lowerTranslation"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("upperTranslation"), wxPG_LABEL, joint->m_upper_translation));
	pg->SetPropertyAttribute(wxT("upperTranslation"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableMotor"), wxPG_LABEL, joint->m_enable_motor));
	pg->SetPropertyAttribute("enableMotor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("maxMotorForce"), wxPG_LABEL, joint->m_max_motor_force));
	pg->SetPropertyAttribute(wxT("maxMotorForce"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("motorSpeed"), wxPG_LABEL, joint->m_motor_speed));
	pg->SetPropertyAttribute(wxT("motorSpeed"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(PrismaticJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("localAxisA.x"))->SetValue(joint->m_local_axis_a.x);
	pg->GetProperty(wxT("localAxisA.y"))->SetValue(joint->m_local_axis_a.y);

	pg->GetProperty(wxT("referenceAngle"))->SetValue(joint->m_reference_angle);

	pg->GetProperty(wxT("enableLimit"))->SetValue(joint->m_enable_limit);

	pg->GetProperty(wxT("lowerTranslation"))->SetValue(joint->m_lower_translation);
	pg->GetProperty(wxT("upperTranslation"))->SetValue(joint->m_upper_translation);

	pg->GetProperty(wxT("enableMotor"))->SetValue(joint->m_enable_motor);

	pg->GetProperty(wxT("maxMotorForce"))->SetValue(joint->m_max_motor_force);

	pg->GetProperty(wxT("motorSpeed"))->SetValue(joint->m_motor_speed);
}

void JointPropertySetting::OnPropertyGridChange(PrismaticJoint* joint, 
												const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("referenceAngle"))
		joint->m_reference_angle = wxANY_AS(value, float);
	else if (name == wxT("enableLimit"))
		joint->m_enable_limit = wxANY_AS(value, bool);
	else if (name == wxT("lowerTranslation"))
		joint->m_lower_translation = wxANY_AS(value, float);
	else if (name == wxT("upperTranslation"))
		joint->m_upper_translation = wxANY_AS(value, float);
	else if (name == wxT("enableMotor"))
		joint->m_enable_motor = wxANY_AS(value, bool);
	else if (name == wxT("maxMotorForce"))
		joint->m_max_motor_force = wxANY_AS(value, float);
	else if (name == wxT("motorSpeed"))
		joint->m_motor_speed = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(DistanceJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("frequencyHz"), wxPG_LABEL, joint->m_frequency_hz));
	pg->SetPropertyAttribute(wxT("frequencyHz"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("dampingRatio"), wxPG_LABEL, joint->m_damping_ratio));
	pg->SetPropertyAttribute(wxT("dampingRatio"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(DistanceJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("frequencyHz"))->SetValue(joint->m_frequency_hz);

	pg->GetProperty(wxT("dampingRatio"))->SetValue(joint->m_damping_ratio);
}

void JointPropertySetting::OnPropertyGridChange(DistanceJoint* joint, const std::string& name, const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("frequencyHz"))
		joint->m_frequency_hz = wxANY_AS(value, float);
	else if (name == wxT("dampingRatio"))
		joint->m_damping_ratio = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(PulleyJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	wxPGProperty* groundAnchorAProp = pg->Append(new wxStringProperty(wxT("groundAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(groundAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_ground_anchor_a.x));
	pg->SetPropertyAttribute(wxT("groundAnchorA.x"), "Precision", 2);
	pg->AppendIn(groundAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_ground_anchor_a.y));
	pg->SetPropertyAttribute(wxT("groundAnchorA.y"), "Precision", 2);

	wxPGProperty* groundAnchorBProp = pg->Append(new wxStringProperty(wxT("groundAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(groundAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_ground_anchor_b.x));
	pg->SetPropertyAttribute(wxT("groundAnchorB.x"), "Precision", 2);
	pg->AppendIn(groundAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_ground_anchor_b.y));
	pg->SetPropertyAttribute(wxT("groundAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("ratio"), wxPG_LABEL, joint->m_ratio));
	pg->SetPropertyAttribute(wxT("ratio"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(PulleyJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("groundAnchorA.x"))->SetValue(joint->m_ground_anchor_a.x);
	pg->GetProperty(wxT("groundAnchorA.y"))->SetValue(joint->m_ground_anchor_a.y);

	pg->GetProperty(wxT("groundAnchorB.x"))->SetValue(joint->m_ground_anchor_b.x);
	pg->GetProperty(wxT("groundAnchorB.y"))->SetValue(joint->m_ground_anchor_b.y);

	pg->GetProperty(wxT("ratio"))->SetValue(joint->m_ratio);
}

void JointPropertySetting::OnPropertyGridChange(PulleyJoint* joint, 
												const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorA.x"))
		joint->m_ground_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorA.y"))
		joint->m_ground_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorB.x"))
		joint->m_ground_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorB.y"))
		joint->m_ground_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("ratio"))
		joint->m_ratio = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(GearJoint* joint, wxPropertyGrid* pg)
{
	pg->Append(new wxFloatProperty(wxT("ratio"), wxPG_LABEL, joint->m_ratio));
	pg->SetPropertyAttribute(wxT("ratio"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(GearJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("ratio"))->SetValue(joint->m_ratio);
}

void JointPropertySetting::OnPropertyGridChange(GearJoint* joint, 
												const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("ratio"))
		joint->m_ratio = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(WheelJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	wxPGProperty* localAxisAProp = pg->Append(new wxStringProperty(wxT("localAxisA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_axis_a.x));
	pg->SetPropertyAttribute(wxT("localAxisA.x"), "Precision", 2);
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_axis_a.y));
	pg->SetPropertyAttribute(wxT("localAxisA.y"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableMotor"), wxPG_LABEL, joint->m_enable_motor));
	pg->SetPropertyAttribute("enableMotor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("maxMotorTorque"), wxPG_LABEL, joint->m_max_motor_torque));
	pg->SetPropertyAttribute(wxT("maxMotorTorque"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("motorSpeed"), wxPG_LABEL, joint->m_motor_speed));
	pg->SetPropertyAttribute(wxT("motorSpeed"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("frequencyHz"), wxPG_LABEL, joint->m_frequency_hz));
	pg->SetPropertyAttribute(wxT("frequencyHz"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("dampingRatio"), wxPG_LABEL, joint->m_damping_ratio));
	pg->SetPropertyAttribute(wxT("dampingRatio"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(WheelJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("localAxisA.x"))->SetValue(joint->m_local_axis_a.x);
	pg->GetProperty(wxT("localAxisA.y"))->SetValue(joint->m_local_axis_a.y);

	pg->GetProperty(wxT("enableMotor"))->SetValue(joint->m_enable_motor);
	pg->GetProperty(wxT("maxMotorTorque"))->SetValue(joint->m_max_motor_torque);

	pg->GetProperty(wxT("motorSpeed"))->SetValue(joint->m_motor_speed);

	pg->GetProperty(wxT("frequencyHz"))->SetValue(joint->m_frequency_hz);
	pg->GetProperty(wxT("dampingRatio"))->SetValue(joint->m_damping_ratio);
}

void JointPropertySetting::OnPropertyGridChange(WheelJoint* joint, const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.x"))
		joint->m_local_axis_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.y"))
		joint->m_local_axis_a.y = wxANY_AS(value, float);
	else if (name == wxT("enableMotor"))
		joint->m_enable_motor = wxANY_AS(value, bool);
	else if (name == wxT("maxMotorTorque"))
		joint->m_max_motor_torque = wxANY_AS(value, float);
	else if (name == wxT("motorSpeed"))
		joint->m_motor_speed = wxANY_AS(value, float);
	else if (name == wxT("frequencyHz"))
		joint->m_frequency_hz = wxANY_AS(value, float);
	else if (name == wxT("dampingRatio"))
		joint->m_damping_ratio = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(WeldJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("referenceAngle"), wxPG_LABEL, joint->m_reference_angle));
	pg->SetPropertyAttribute(wxT("referenceAngle"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("frequencyHz"), wxPG_LABEL, joint->m_frequency_hz));
	pg->SetPropertyAttribute(wxT("frequencyHz"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("dampingRatio"), wxPG_LABEL, joint->m_damping_ratio));
	pg->SetPropertyAttribute(wxT("dampingRatio"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(WeldJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("referenceAngle"))->SetValue(joint->m_reference_angle);

	pg->GetProperty(wxT("frequencyHz"))->SetValue(joint->m_frequency_hz);

	pg->GetProperty(wxT("dampingRatio"))->SetValue(joint->m_damping_ratio);
}

void JointPropertySetting::OnPropertyGridChange(WeldJoint* joint, const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("referenceAngle"))
		joint->m_reference_angle = wxANY_AS(value, float);
	else if (name == wxT("frequencyHz"))
		joint->m_frequency_hz = wxANY_AS(value, float);
	else if (name == wxT("dampingRatio"))
		joint->m_damping_ratio = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(FrictionJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxForce"), wxPG_LABEL, joint->m_max_force));
	pg->SetPropertyAttribute(wxT("maxForce"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxTorque"), wxPG_LABEL, joint->m_max_torque));
	pg->SetPropertyAttribute(wxT("maxTorque"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(FrictionJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("maxForce"))->SetValue(joint->m_max_force);

	pg->GetProperty(wxT("maxTorque"))->SetValue(joint->m_max_torque);
}

void JointPropertySetting::OnPropertyGridChange(FrictionJoint* joint, const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("maxForce"))
		joint->m_max_force = wxANY_AS(value, float);
	else if (name == wxT("maxTorque"))
		joint->m_max_torque = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(RopeJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_a.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_a.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->m_local_anchor_b.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->m_local_anchor_b.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxLength"), wxPG_LABEL, joint->m_max_length));
	pg->SetPropertyAttribute(wxT("maxLength"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(RopeJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->m_local_anchor_a.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->m_local_anchor_a.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->m_local_anchor_b.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->m_local_anchor_b.y);

	pg->GetProperty(wxT("maxLength"))->SetValue(joint->m_max_length);
}

void JointPropertySetting::OnPropertyGridChange(RopeJoint* joint, const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->m_local_anchor_a.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->m_local_anchor_a.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->m_local_anchor_b.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->m_local_anchor_b.y = wxANY_AS(value, float);
	else if (name == wxT("maxLength"))
		joint->m_max_length = wxANY_AS(value, float);
}

void JointPropertySetting::CreatePropertyPanel(MotorJoint* joint, wxPropertyGrid* pg)
{
	pg->Append(new wxFloatProperty(wxT("maxForce"), wxPG_LABEL, joint->m_max_force));
	pg->SetPropertyAttribute(wxT("maxForce"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxTorque"), wxPG_LABEL, joint->m_max_torque));
	pg->SetPropertyAttribute(wxT("maxTorque"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("correctionFactor"), wxPG_LABEL, joint->m_correction_factor));
	pg->SetPropertyAttribute(wxT("correctionFactor"), "Precision", 2);
}

void JointPropertySetting::UpdatePropertyPanel(MotorJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("maxForce"))->SetValue(joint->m_max_force);
	pg->GetProperty(wxT("maxTorque"))->SetValue(joint->m_max_torque);
	pg->GetProperty(wxT("correctionFactor"))->SetValue(joint->m_correction_factor);
}

void JointPropertySetting::OnPropertyGridChange(MotorJoint* joint, const std::string& name, 
												const wxAny& value)
{
	if (name == wxT("maxForce"))
		joint->m_max_force = wxANY_AS(value, float);
	else if (name == wxT("maxTorque"))
		joint->m_max_torque = wxANY_AS(value, float);
	else if (name == wxT("correctionFactor"))
		joint->m_correction_factor = wxANY_AS(value, float);
}

}