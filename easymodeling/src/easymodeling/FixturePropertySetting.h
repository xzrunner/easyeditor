#ifndef _EASYMODELING_FIXTURE_PROPERTY_SETTING_H_
#define _EASYMODELING_FIXTURE_PROPERTY_SETTING_H_



namespace libmodeling { class Fixture; }

namespace emodeling
{

class FixturePropertySetting : public ee::PropertySetting
{
public:
	FixturePropertySetting(ee::EditPanelImpl* stage, libmodeling::Fixture* fixture);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	libmodeling::Fixture* m_fixture;

}; // FixturePropertySetting

}

#endif // _EASYMODELING_FIXTURE_PROPERTY_SETTING_H_
