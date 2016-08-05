#ifndef _EASYTRAIL_IMAGE_COMPONENT_PANEL_H_
#define _EASYTRAIL_IMAGE_COMPONENT_PANEL_H_

#include "ComponentPanel.h"

namespace etrail
{

class ImageCompPanel : public ComponentPanel
{
public:
	ImageCompPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val) const;

protected:
	virtual void InitLayout(wxSizer* sizer);
	virtual std::string GetTitle() const;

private:
	void OnDelete(wxCommandEvent& event);

}; // ImageCompPanel

}

#endif // _EASYTRAIL_IMAGE_COMPONENT_PANEL_H_
