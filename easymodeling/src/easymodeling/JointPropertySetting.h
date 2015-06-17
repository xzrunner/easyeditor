#ifndef _EASYMODELING_JOINT_PROPERTY_SETTING_H_
#define _EASYMODELING_JOINT_PROPERTY_SETTING_H_

#include <drag2d.h>

#include <easymodeling.h>

namespace emodeling
{

class JointPropertySetting : public d2d::IPropertySetting
{
public:
	JointPropertySetting(d2d::EditPanel* stage, libmodeling::Joint* joint);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	void CreatePropertyPanel(libmodeling::RevoluteJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::RevoluteJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::RevoluteJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::PrismaticJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::PrismaticJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::PrismaticJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::DistanceJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::DistanceJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::DistanceJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::PulleyJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::PulleyJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::PulleyJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::GearJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::GearJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::GearJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::WheelJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::WheelJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::WheelJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::WeldJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::WeldJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::WeldJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::FrictionJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::FrictionJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::FrictionJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::RopeJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::RopeJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::RopeJoint* joint,
		const wxString& name, const wxAny& value);

	void CreatePropertyPanel(libmodeling::MotorJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(libmodeling::MotorJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(libmodeling::MotorJoint* joint,
		const wxString& name, const wxAny& value);

private:
	d2d::EditPanel* m_stage;

	libmodeling::Joint* m_joint;

}; // JointPropertySetting

}

#endif // _EASYMODELING_JOINT_PROPERTY_SETTING_H_
