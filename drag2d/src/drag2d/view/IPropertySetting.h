#ifndef _DRAG2D_IPROPERTY_SETTING_H_
#define _DRAG2D_IPROPERTY_SETTING_H_

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

namespace d2d
{

class PropertySettingPanel;

class IPropertySetting
{
public:
	IPropertySetting(const std::string& type);
	virtual ~IPropertySetting() {}

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value) = 0;

	void UpdateFromPanel(PropertySettingPanel* panel);
	void EnablePropertyGrid(PropertySettingPanel* panel, bool enable);

	const std::string& GetType() const { return m_type; }

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) = 0;
	virtual void InitProperties(wxPropertyGrid* pg) = 0;

	wxString GetPGType(wxPropertyGrid* pg) const;

	static void SplitString2Double(const wxAny& value, double* x, double* y);

protected:
	static const wxString TYPE_NAME;

protected:
	std::string m_type;

}; // IPropertySetting

}

#endif // _DRAG2D_IPROPERTY_SETTING_H_