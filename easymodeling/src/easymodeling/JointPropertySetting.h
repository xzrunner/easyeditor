#ifndef _EASYMODELING_JOINT_PROPERTY_SETTING_H_
#define _EASYMODELING_JOINT_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

#include <easymodeling.h>

namespace ee { class EditPanelImpl; }

namespace emodeling
{

class JointPropertySetting : public ee::PropertySetting
{
public:
	JointPropertySetting(ee::EditPanelImpl* stage, Joint* joint);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	void CreatePropertyPanel(RevoluteJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(RevoluteJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(RevoluteJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(PrismaticJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(PrismaticJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(PrismaticJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(DistanceJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(DistanceJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(DistanceJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(PulleyJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(PulleyJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(PulleyJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(GearJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(GearJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(GearJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(WheelJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(WheelJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(WheelJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(WeldJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(WeldJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(WeldJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(FrictionJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(FrictionJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(FrictionJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(RopeJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(RopeJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(RopeJoint* joint,
		const std::string& name, const wxAny& value);

	void CreatePropertyPanel(MotorJoint* joint, wxPropertyGrid* pg);
	void UpdatePropertyPanel(MotorJoint* joint, wxPropertyGrid* pg);
	void OnPropertyGridChange(MotorJoint* joint,
		const std::string& name, const wxAny& value);

private:
	ee::EditPanelImpl* m_stage;

	Joint* m_joint;

}; // JointPropertySetting

}

#endif // _EASYMODELING_JOINT_PROPERTY_SETTING_H_
