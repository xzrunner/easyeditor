#ifndef _EASYTRAIL_SHAPE_COMPONENT_PANEL_H_
#define _EASYTRAIL_SHAPE_COMPONENT_PANEL_H_

#include "ComponentPanel.h"

#include <painting2/Color.h>

class wxButton;

namespace etrail
{

class ShapeCompPanel : public ComponentPanel
{
public:
	ShapeCompPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data) override;
	virtual void GetValue(int key, ee::UICallback::Data& data) override;

	virtual void Load(const Json::Value& val, const std::string& dir) override;
	virtual void Store(Json::Value& val, const std::string& dir) const override;

protected:
	virtual void InitLayout(wxSizer* sizer) override;
	virtual std::string GetTitle() const override { return ""; }

	virtual void UpdateBtnColor() override;

private:
	void OnSetBeginColor(wxCommandEvent& event);
	void OnSetEndColor(wxCommandEvent& event);

private:
	wxButton *m_begin_col_btn, *m_end_col_btn;

}; // ShapeCompPanel

}

#endif // _EASYTRAIL_SHAPE_COMPONENT_PANEL_H_