
#include "JointPropertySetting.h"

using namespace emodeling;

JointPropertySetting::JointPropertySetting(d2d::EditPanel* editPanel, libmodeling::Joint* joint)
	: d2d::IPropertySetting(editPanel, wxT("Joint"))
{
	m_joint = joint;

	switch (joint->type)
	{
	case libmodeling::Joint::e_revoluteJoint:
		m_type = wxT("Revolute Joint");
		break;
	case libmodeling::Joint::e_prismaticJoint:
		m_type = wxT("Prismatic Joint");
		break;
	case libmodeling::Joint::e_distanceJoint:
		m_type = wxT("Distance Joint");
		break;
	case libmodeling::Joint::e_pulleyJoint:
		m_type = wxT("Pulley Joint");
		break;
	case libmodeling::Joint::e_mouseJoint:
		m_type = wxT("Mouse Joint");
		break;
	case libmodeling::Joint::e_gearJoint:
		m_type = wxT("Gear Joint");
		break;
	case libmodeling::Joint::e_wheelJoint:
		m_type = wxT("Wheel Joint");
		break;
	case libmodeling::Joint::e_weldJoint:
		m_type = wxT("Weld Joint");
		break;
	case libmodeling::Joint::e_frictionJoint:
		m_type = wxT("Friction Joint");
		break;
	case libmodeling::Joint::e_ropeJoint:
		m_type = wxT("Rope Joint");
		break;
	case libmodeling::Joint::e_motorJoint:
		m_type = wxT("Motor Joint");
		break;
	}
}

void JointPropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	bool build;
	if (getPGType(pg) == m_type)
	{
		build = false;
		pg->GetProperty(wxT("Name"))->SetValue(m_joint->m_name);
		pg->GetProperty(wxT("collideConnected"))->SetValue(m_joint->collideConnected);
	}
	else
	{
		build = true;
		pg->Clear();
		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));
		pg->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, m_joint->m_name));
		pg->Append(new wxBoolProperty(wxT("collideConnected"), wxPG_LABEL, m_joint->collideConnected));
		pg->SetPropertyAttribute("collideConnected", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	}

	switch (m_joint->type)
	{
	case libmodeling::Joint::e_revoluteJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::RevoluteJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::RevoluteJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_prismaticJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::PrismaticJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::PrismaticJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_distanceJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::DistanceJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::DistanceJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_pulleyJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::PulleyJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::PulleyJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_gearJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::GearJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::GearJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_wheelJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::WheelJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::WheelJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_weldJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::WeldJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::WeldJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_frictionJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::FrictionJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::FrictionJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_ropeJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::RopeJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::RopeJoint*>(m_joint), pg);
		break;
	case libmodeling::Joint::e_motorJoint:
		if (build) 
			createPropertyPanel(static_cast<libmodeling::MotorJoint*>(m_joint), pg);
		else
			updatePropertyPanel(static_cast<libmodeling::MotorJoint*>(m_joint), pg);
		break;
	}
}

void JointPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("Name"))
		m_joint->m_name = wxANY_AS(value, wxString);
	else if (name == wxT("collideConnected"))
		m_joint->collideConnected = wxANY_AS(value, bool);
	switch (m_joint->type)
	{
	case libmodeling::Joint::e_revoluteJoint:
		onPropertyGridChange(static_cast<libmodeling::RevoluteJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_prismaticJoint:
		onPropertyGridChange(static_cast<libmodeling::PrismaticJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_distanceJoint:
		onPropertyGridChange(static_cast<libmodeling::DistanceJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_pulleyJoint:
		onPropertyGridChange(static_cast<libmodeling::PulleyJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_gearJoint:
		onPropertyGridChange(static_cast<libmodeling::GearJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_wheelJoint:
		onPropertyGridChange(static_cast<libmodeling::WheelJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_weldJoint:
		onPropertyGridChange(static_cast<libmodeling::WeldJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_frictionJoint:
		onPropertyGridChange(static_cast<libmodeling::FrictionJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_ropeJoint:
		onPropertyGridChange(static_cast<libmodeling::RopeJoint*>(m_joint), name, value);
		break;
	case libmodeling::Joint::e_motorJoint:
		onPropertyGridChange(static_cast<libmodeling::MotorJoint*>(m_joint), name, value);
		break;
	}

	m_editPanel->Refresh();
}

void JointPropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void JointPropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	// 
}

void JointPropertySetting::createPropertyPanel(libmodeling::RevoluteJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("referenceAngle"), wxPG_LABEL, joint->referenceAngle));
	pg->SetPropertyAttribute(wxT("referenceAngle"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableLimit"), wxPG_LABEL, joint->enableLimit));
	pg->SetPropertyAttribute("enableLimit", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("lowerAngle"), wxPG_LABEL, joint->lowerAngle));
	pg->SetPropertyAttribute(wxT("lowerAngle"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("upperAngle"), wxPG_LABEL, joint->upperAngle));
	pg->SetPropertyAttribute(wxT("upperAngle"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableMotor"), wxPG_LABEL, joint->enableMotor));
	pg->SetPropertyAttribute("enableMotor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("maxMotorTorque"), wxPG_LABEL, joint->maxMotorTorque));
	pg->SetPropertyAttribute(wxT("maxMotorTorque"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("motorSpeed"), wxPG_LABEL, joint->motorSpeed));
	pg->SetPropertyAttribute(wxT("motorSpeed"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::RevoluteJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("referenceAngle"))->SetValue(joint->referenceAngle);

	pg->GetProperty(wxT("enableLimit"))->SetValue(joint->enableLimit);

	pg->GetProperty(wxT("lowerAngle"))->SetValue(joint->lowerAngle);
	pg->GetProperty(wxT("upperAngle"))->SetValue(joint->upperAngle);

	pg->GetProperty(wxT("enableMotor"))->SetValue(joint->enableMotor);

	pg->GetProperty(wxT("maxMotorTorque"))->SetValue(joint->maxMotorTorque);

	pg->GetProperty(wxT("motorSpeed"))->SetValue(joint->motorSpeed);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::RevoluteJoint* joint, 
												const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("referenceAngle"))
		joint->referenceAngle = wxANY_AS(value, float);
	else if (name == wxT("enableLimit"))
		joint->enableLimit = wxANY_AS(value, bool);
	else if (name == wxT("lowerAngle"))
		joint->lowerAngle = wxANY_AS(value, float);
	else if (name == wxT("upperAngle"))
		joint->upperAngle = wxANY_AS(value, float);
	else if (name == wxT("enableMotor"))
		joint->enableMotor = wxANY_AS(value, bool);
	else if (name == wxT("maxMotorTorque"))
		joint->maxMotorTorque = wxANY_AS(value, float);
	else if (name == wxT("motorSpeed"))
		joint->motorSpeed = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::PrismaticJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	wxPGProperty* localAxisAProp = pg->Append(new wxStringProperty(wxT("localAxisA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAxisA.x));
	pg->SetPropertyAttribute(wxT("localAxisA.x"), "Precision", 2);
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAxisA.y));
	pg->SetPropertyAttribute(wxT("localAxisA.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("referenceAngle"), wxPG_LABEL, joint->referenceAngle));
	pg->SetPropertyAttribute(wxT("referenceAngle"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableLimit"), wxPG_LABEL, joint->enableLimit));
	pg->SetPropertyAttribute("enableLimit", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("lowerTranslation"), wxPG_LABEL, joint->lowerTranslation));
	pg->SetPropertyAttribute(wxT("lowerTranslation"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("upperTranslation"), wxPG_LABEL, joint->upperTranslation));
	pg->SetPropertyAttribute(wxT("upperTranslation"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableMotor"), wxPG_LABEL, joint->enableMotor));
	pg->SetPropertyAttribute("enableMotor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("maxMotorForce"), wxPG_LABEL, joint->maxMotorForce));
	pg->SetPropertyAttribute(wxT("maxMotorForce"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("motorSpeed"), wxPG_LABEL, joint->motorSpeed));
	pg->SetPropertyAttribute(wxT("motorSpeed"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::PrismaticJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("localAxisA.x"))->SetValue(joint->localAxisA.x);
	pg->GetProperty(wxT("localAxisA.y"))->SetValue(joint->localAxisA.y);

	pg->GetProperty(wxT("referenceAngle"))->SetValue(joint->referenceAngle);

	pg->GetProperty(wxT("enableLimit"))->SetValue(joint->enableLimit);

	pg->GetProperty(wxT("lowerTranslation"))->SetValue(joint->lowerTranslation);
	pg->GetProperty(wxT("upperTranslation"))->SetValue(joint->upperTranslation);

	pg->GetProperty(wxT("enableMotor"))->SetValue(joint->enableMotor);

	pg->GetProperty(wxT("maxMotorForce"))->SetValue(joint->maxMotorForce);

	pg->GetProperty(wxT("motorSpeed"))->SetValue(joint->motorSpeed);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::PrismaticJoint* joint, 
												const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.x"))
		joint->localAxisA.x = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.y"))
		joint->localAxisA.y = wxANY_AS(value, float);
	else if (name == wxT("referenceAngle"))
		joint->referenceAngle = wxANY_AS(value, float);
	else if (name == wxT("enableLimit"))
		joint->enableLimit = wxANY_AS(value, bool);
	else if (name == wxT("lowerTranslation"))
		joint->lowerTranslation = wxANY_AS(value, float);
	else if (name == wxT("upperTranslation"))
		joint->upperTranslation = wxANY_AS(value, float);
	else if (name == wxT("enableMotor"))
		joint->enableMotor = wxANY_AS(value, bool);
	else if (name == wxT("maxMotorForce"))
		joint->maxMotorForce = wxANY_AS(value, float);
	else if (name == wxT("motorSpeed"))
		joint->motorSpeed = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::DistanceJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("frequencyHz"), wxPG_LABEL, joint->frequencyHz));
	pg->SetPropertyAttribute(wxT("frequencyHz"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("dampingRatio"), wxPG_LABEL, joint->dampingRatio));
	pg->SetPropertyAttribute(wxT("dampingRatio"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::DistanceJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("frequencyHz"))->SetValue(joint->frequencyHz);

	pg->GetProperty(wxT("dampingRatio"))->SetValue(joint->dampingRatio);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::DistanceJoint* joint, const wxString& name, const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("frequencyHz"))
		joint->frequencyHz = wxANY_AS(value, float);
	else if (name == wxT("dampingRatio"))
		joint->dampingRatio = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::PulleyJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	wxPGProperty* groundAnchorAProp = pg->Append(new wxStringProperty(wxT("groundAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(groundAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->groundAnchorA.x));
	pg->SetPropertyAttribute(wxT("groundAnchorA.x"), "Precision", 2);
	pg->AppendIn(groundAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->groundAnchorA.y));
	pg->SetPropertyAttribute(wxT("groundAnchorA.y"), "Precision", 2);

	wxPGProperty* groundAnchorBProp = pg->Append(new wxStringProperty(wxT("groundAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(groundAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->groundAnchorB.x));
	pg->SetPropertyAttribute(wxT("groundAnchorB.x"), "Precision", 2);
	pg->AppendIn(groundAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->groundAnchorB.y));
	pg->SetPropertyAttribute(wxT("groundAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("ratio"), wxPG_LABEL, joint->ratio));
	pg->SetPropertyAttribute(wxT("ratio"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::PulleyJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("groundAnchorA.x"))->SetValue(joint->groundAnchorA.x);
	pg->GetProperty(wxT("groundAnchorA.y"))->SetValue(joint->groundAnchorA.y);

	pg->GetProperty(wxT("groundAnchorB.x"))->SetValue(joint->groundAnchorB.x);
	pg->GetProperty(wxT("groundAnchorB.y"))->SetValue(joint->groundAnchorB.y);

	pg->GetProperty(wxT("ratio"))->SetValue(joint->ratio);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::PulleyJoint* joint, 
												const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorA.x"))
		joint->groundAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorA.y"))
		joint->groundAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorB.x"))
		joint->groundAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("groundAnchorB.y"))
		joint->groundAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("ratio"))
		joint->ratio = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::GearJoint* joint, wxPropertyGrid* pg)
{
	pg->Append(new wxFloatProperty(wxT("ratio"), wxPG_LABEL, joint->ratio));
	pg->SetPropertyAttribute(wxT("ratio"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::GearJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("ratio"))->SetValue(joint->ratio);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::GearJoint* joint, 
												const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("ratio"))
		joint->ratio = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::WheelJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	wxPGProperty* localAxisAProp = pg->Append(new wxStringProperty(wxT("localAxisA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAxisA.x));
	pg->SetPropertyAttribute(wxT("localAxisA.x"), "Precision", 2);
	pg->AppendIn(localAxisAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAxisA.y));
	pg->SetPropertyAttribute(wxT("localAxisA.y"), "Precision", 2);

	pg->Append(new wxBoolProperty(wxT("enableMotor"), wxPG_LABEL, joint->enableMotor));
	pg->SetPropertyAttribute("enableMotor", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);

	pg->Append(new wxFloatProperty(wxT("maxMotorTorque"), wxPG_LABEL, joint->maxMotorTorque));
	pg->SetPropertyAttribute(wxT("maxMotorTorque"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("motorSpeed"), wxPG_LABEL, joint->motorSpeed));
	pg->SetPropertyAttribute(wxT("motorSpeed"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("frequencyHz"), wxPG_LABEL, joint->frequencyHz));
	pg->SetPropertyAttribute(wxT("frequencyHz"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("dampingRatio"), wxPG_LABEL, joint->dampingRatio));
	pg->SetPropertyAttribute(wxT("dampingRatio"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::WheelJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("localAxisA.x"))->SetValue(joint->localAxisA.x);
	pg->GetProperty(wxT("localAxisA.y"))->SetValue(joint->localAxisA.y);

	pg->GetProperty(wxT("enableMotor"))->SetValue(joint->enableMotor);
	pg->GetProperty(wxT("maxMotorTorque"))->SetValue(joint->maxMotorTorque);

	pg->GetProperty(wxT("motorSpeed"))->SetValue(joint->motorSpeed);

	pg->GetProperty(wxT("frequencyHz"))->SetValue(joint->frequencyHz);
	pg->GetProperty(wxT("dampingRatio"))->SetValue(joint->dampingRatio);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::WheelJoint* joint, const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.x"))
		joint->localAxisA.x = wxANY_AS(value, float);
	else if (name == wxT("localAxisA.y"))
		joint->localAxisA.y = wxANY_AS(value, float);
	else if (name == wxT("enableMotor"))
		joint->enableMotor = wxANY_AS(value, bool);
	else if (name == wxT("maxMotorTorque"))
		joint->maxMotorTorque = wxANY_AS(value, float);
	else if (name == wxT("motorSpeed"))
		joint->motorSpeed = wxANY_AS(value, float);
	else if (name == wxT("frequencyHz"))
		joint->frequencyHz = wxANY_AS(value, float);
	else if (name == wxT("dampingRatio"))
		joint->dampingRatio = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::WeldJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("referenceAngle"), wxPG_LABEL, joint->referenceAngle));
	pg->SetPropertyAttribute(wxT("referenceAngle"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("frequencyHz"), wxPG_LABEL, joint->frequencyHz));
	pg->SetPropertyAttribute(wxT("frequencyHz"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("dampingRatio"), wxPG_LABEL, joint->dampingRatio));
	pg->SetPropertyAttribute(wxT("dampingRatio"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::WeldJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("referenceAngle"))->SetValue(joint->referenceAngle);

	pg->GetProperty(wxT("frequencyHz"))->SetValue(joint->frequencyHz);

	pg->GetProperty(wxT("dampingRatio"))->SetValue(joint->dampingRatio);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::WeldJoint* joint, const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("referenceAngle"))
		joint->referenceAngle = wxANY_AS(value, float);
	else if (name == wxT("frequencyHz"))
		joint->frequencyHz = wxANY_AS(value, float);
	else if (name == wxT("dampingRatio"))
		joint->dampingRatio = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::FrictionJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxForce"), wxPG_LABEL, joint->maxForce));
	pg->SetPropertyAttribute(wxT("maxForce"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxTorque"), wxPG_LABEL, joint->maxTorque));
	pg->SetPropertyAttribute(wxT("maxTorque"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::FrictionJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("maxForce"))->SetValue(joint->maxForce);

	pg->GetProperty(wxT("maxTorque"))->SetValue(joint->maxTorque);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::FrictionJoint* joint, const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("maxForce"))
		joint->maxForce = wxANY_AS(value, float);
	else if (name == wxT("maxTorque"))
		joint->maxTorque = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::RopeJoint* joint, wxPropertyGrid* pg)
{
	wxPGProperty* localAnchorAProp = pg->Append(new wxStringProperty(wxT("localAnchorA"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorA.x));
	pg->SetPropertyAttribute(wxT("localAnchorA.x"), "Precision", 2);
	pg->AppendIn(localAnchorAProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorA.y));
	pg->SetPropertyAttribute(wxT("localAnchorA.y"), "Precision", 2);

	wxPGProperty* localAnchorBProp = pg->Append(new wxStringProperty(wxT("localAnchorB"),
		wxPG_LABEL, wxT("<composed>")));
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("x"), wxPG_LABEL, joint->localAnchorB.x));
	pg->SetPropertyAttribute(wxT("localAnchorB.x"), "Precision", 2);
	pg->AppendIn(localAnchorBProp, new wxFloatProperty(wxT("y"), wxPG_LABEL, joint->localAnchorB.y));
	pg->SetPropertyAttribute(wxT("localAnchorB.y"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxLength"), wxPG_LABEL, joint->maxLength));
	pg->SetPropertyAttribute(wxT("maxLength"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::RopeJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("localAnchorA.x"))->SetValue(joint->localAnchorA.x);
	pg->GetProperty(wxT("localAnchorA.y"))->SetValue(joint->localAnchorA.y);

	pg->GetProperty(wxT("localAnchorB.x"))->SetValue(joint->localAnchorB.x);
	pg->GetProperty(wxT("localAnchorB.y"))->SetValue(joint->localAnchorB.y);

	pg->GetProperty(wxT("maxLength"))->SetValue(joint->maxLength);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::RopeJoint* joint, const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("localAnchorA.x"))
		joint->localAnchorA.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorA.y"))
		joint->localAnchorA.y = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.x"))
		joint->localAnchorB.x = wxANY_AS(value, float);
	else if (name == wxT("localAnchorB.y"))
		joint->localAnchorB.y = wxANY_AS(value, float);
	else if (name == wxT("maxLength"))
		joint->maxLength = wxANY_AS(value, float);
}

void JointPropertySetting::createPropertyPanel(libmodeling::MotorJoint* joint, wxPropertyGrid* pg)
{
	pg->Append(new wxFloatProperty(wxT("maxForce"), wxPG_LABEL, joint->maxForce));
	pg->SetPropertyAttribute(wxT("maxForce"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("maxTorque"), wxPG_LABEL, joint->maxTorque));
	pg->SetPropertyAttribute(wxT("maxTorque"), "Precision", 2);

	pg->Append(new wxFloatProperty(wxT("correctionFactor"), wxPG_LABEL, joint->correctionFactor));
	pg->SetPropertyAttribute(wxT("correctionFactor"), "Precision", 2);
}

void JointPropertySetting::updatePropertyPanel(libmodeling::MotorJoint* joint, wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("maxForce"))->SetValue(joint->maxForce);
	pg->GetProperty(wxT("maxTorque"))->SetValue(joint->maxTorque);
	pg->GetProperty(wxT("correctionFactor"))->SetValue(joint->correctionFactor);
}

void JointPropertySetting::onPropertyGridChange(libmodeling::MotorJoint* joint, const wxString& name, 
												const wxAny& value)
{
	if (name == wxT("maxForce"))
		joint->maxForce = wxANY_AS(value, float);
	else if (name == wxT("maxTorque"))
		joint->maxTorque = wxANY_AS(value, float);
	else if (name == wxT("correctionFactor"))
		joint->correctionFactor = wxANY_AS(value, float);
}