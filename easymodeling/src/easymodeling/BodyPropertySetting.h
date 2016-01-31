#ifndef _EASYMODELING_BODY_PROPERTY_SETTING_H_
#define _EASYMODELING_BODY_PROPERTY_SETTING_H_



namespace libmodeling { class Body; }

namespace emodeling
{

class BodyPropertySetting : public ee::PropertySetting
{
public:
	BodyPropertySetting(ee::EditPanelImpl* stage, ee::Sprite* sprite);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	libmodeling::Body* m_body;

}; // BodyPropertySetting

}

#endif // _EASYMODELING_BODY_PROPERTY_SETTING_H_
