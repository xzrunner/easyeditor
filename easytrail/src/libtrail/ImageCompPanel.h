#ifndef _EASYTRAIL_IMAGE_COMPONENT_PANEL_H_
#define _EASYTRAIL_IMAGE_COMPONENT_PANEL_H_

#include "ComponentPanel.h"

class wxButton;

namespace etrail
{

class ImageCompPanel : public ComponentPanel
{
public:
	ImageCompPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data) override;
	virtual void GetValue(int key, ee::UICallback::Data& data) override;

	virtual void Load(const Json::Value& val, const std::string& dir) override;
	virtual void Store(Json::Value& val, const std::string& dir) const override;

protected:
	virtual void InitLayout(wxSizer* sizer) override;
	virtual std::string GetTitle() const override;

	virtual void UpdateBtnColor() override;

private:
	void OnSetBeginMulCol(wxCommandEvent& event);
	void OnSetEndMulCol(wxCommandEvent& event);
	void OnSetBeginAddCol(wxCommandEvent& event);
	void OnSetEndAddCol(wxCommandEvent& event);

	void OnSetColor(uint8_t rgba[4]);

private:
	wxButton *m_begin_mul_col_btn, *m_end_mul_col_btn;
	wxButton *m_begin_add_col_btn, *m_end_add_col_btn;

}; // ImageCompPanel

}

#endif // _EASYTRAIL_IMAGE_COMPONENT_PANEL_H_
