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
	DynamicWidget();
	virtual ~DynamicWidget() {}

	virtual std::string GetValue() const = 0;
	virtual bool IsChanged() const = 0;
	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer, const DynamicInfo& info) = 0;

	virtual void LoadFromFile(const Json::Value& value);

	const std::string& GetTitle() const { return m_title; }
	const std::string& GetKey() const { return m_key; }

	void SetTag(const std::string& tag) { m_tag = tag; }
	const std::string& GetTag() const { return m_tag; }

protected:
	virtual void Load(const Json::Value& value) = 0;

protected:
	std::string m_title;
	std::string m_key;
	std::string m_tag;

	bool m_special;

}; // DynamicWidget

}

#endif // _LR_DYNAMIC_WIDGET_H_