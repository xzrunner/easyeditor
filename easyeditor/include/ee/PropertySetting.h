#ifndef _EASYEDITOR_PROPERTY_SETTING_H_
#define _EASYEDITOR_PROPERTY_SETTING_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

namespace ee
{

class PropertySettingPanel;

class PropertySetting
{
public:
	PropertySetting(const std::string& type);
	virtual ~PropertySetting() {}

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) = 0;

	void UpdateFromPanel(PropertySettingPanel* panel);
	void EnablePropertyGrid(PropertySettingPanel* panel, bool enable);

	const std::string& GetType() const { return m_type; }

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) = 0;
	virtual void InitProperties(wxPropertyGrid* pg) = 0;

	std::string GetPGType(wxPropertyGrid* pg) const;

	static void SplitString2Double(const wxAny& value, double* x, double* y);

protected:
	static const std::string TYPE_NAME;

protected:
	std::string m_type;

}; // PropertySetting

}

#endif // _EASYEDITOR_PROPERTY_SETTING_H_