#ifndef _EASYTRAIL_SHAPE_COMPONENT_PANEL_H_
#define _EASYTRAIL_SHAPE_COMPONENT_PANEL_H_

#include "ComponentPanel.h"

#include <sprite2/Color.h>

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
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

protected:
	virtual void InitLayout(wxSizer* sizer);
	virtual std::string GetTitle() const { return ""; }

private:
	void OnSetColor(wxCommandEvent& event);

	void SetBtnColor();

private:
	wxButton* m_col_btn;

}; // ShapeCompPanel

}

#endif // _EASYTRAIL_SHAPE_COMPONENT_PANEL_H_