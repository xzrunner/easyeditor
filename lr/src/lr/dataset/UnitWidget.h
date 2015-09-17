#ifndef _LR_UNIT_WIDGET_H_
#define _LR_UNIT_WIDGET_H_

#include <json/json.h>
#include <wx/wx.h>

namespace lr
{

class UnitInfo;

class UnitWidget
{
public:
	virtual ~UnitWidget() {}

	virtual void LoadFromFile(const Json::Value& value);

	const std::string& GetTitle() const { return m_title; }

	const std::string& GetKey() const { return m_key; }
	virtual std::string GetValue() const = 0;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const UnitInfo& info) = 0;

protected:
	virtual void Load(const Json::Value& value) = 0;

protected:
	std::string m_title;
	std::string m_key;

	bool m_special;

}; // UnitWidget

class UnitTextWidget : public UnitWidget
{
public:
	virtual std::string GetValue() const;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const UnitInfo& info);

protected:
	virtual void Load(const Json::Value& value);
	
private:
	std::string m_default;

	wxTextCtrl* m_ctrl;

}; // UnitTextWidget

class UnitChoiceWidget : public UnitWidget
{
public:
	virtual std::string GetValue() const;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const UnitInfo& info);

	void SpecialLoad(const std::string& title, const std::string& key,
		const std::vector<std::string>& items, const std::string& default_item);

protected:
	virtual void Load(const Json::Value& value);

private:
	struct Item
	{
		std::string value;
		std::string title;
	};

	static const int MAX_ITEMS = 100;

private:
	int m_default;

	std::vector<Item> m_choices;

	wxChoice* m_ctrl;

}; // UnitChoiceWidget

class UnitComboBoxWidget : public UnitWidget
{
public:
	virtual std::string GetValue() const;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const UnitInfo& info);

protected:
	virtual void Load(const Json::Value& value);

private:
	void OnText(wxCommandEvent& event);

private:
	struct Item
	{
		std::string value;
		std::string title;
	};

	static const int MAX_ITEMS = 100;

private:
	int m_default;

	std::vector<Item> m_choices;

	wxComboBox* m_ctrl;

}; // UnitComboBoxWidget

class UnitCheckBoxWidget : public UnitWidget
{
public:
	virtual std::string GetValue() const;

	virtual void InitLayout(wxWindow* parent, wxSizer* top_sizer,
		const UnitInfo& info);

protected:
	virtual void Load(const Json::Value& value);
	
private:
	bool m_default;	

	wxCheckBox* m_ctrl;

}; // UnitCheckBoxWidget

}

#endif // _LR_UNIT_WIDGET_H_
