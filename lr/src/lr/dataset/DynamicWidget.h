#ifndef _LR_DYNAMIC_WIDGET_H_
#define _LR_DYNAMIC_WIDGET_H_

#include <json/json.h>
#include <wx/wx.h>

namespace lr
{

class DynamicInfo;

class DynamicWidget
{
public:
	virtual ~DynamicWidget() {}

	virtual void LoadFromFile(const Json::Value& value);

	const std::string& GetTitle() const { return m_title; }

	const std::string& GetKey() const { return m_key; }

	virtual std::string GetValue() const = 0;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const DynamicInfo& info) = 0;

protected:
	virtual void Load(const Json::Value& value) = 0;

protected:
	std::string m_title;
	std::string m_key;

	bool m_special;

}; // DynamicWidget

}

#endif // _LR_DYNAMIC_WIDGET_H_