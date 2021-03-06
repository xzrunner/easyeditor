#include "ComponentPanel.h"
#include "ToolbarPanel.h"
#include "mt_config.h"

#include <ee/Symbol.h>
#include <ee/SliderCtrl.h>

#include <mt_2d.h>

#include <wx/colourdata.h>
#include <wx/colordlg.h>

namespace etrail
{

ComponentPanel::ComponentPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar)	
	: wxPanel(parent)
	, m_pc(pc)
	, m_toolbar(toolbar) 
{}

void ComponentPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	switch (key)
	{
	case MT_ALPHA:
		m_pc->col_begin.a = data.val0;
		m_pc->col_end.a = data.val1;
		break;
	}
}

void ComponentPanel::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case MT_ALPHA:
		data.val0 = m_pc->col_begin.a;
		data.val1 = m_pc->col_end.a;
		break;
	}
}

void ComponentPanel::Load(const Json::Value& val, const std::string& dir)
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Load(val, 0);
		m_sliders[i]->Update();
	}
}

void ComponentPanel::Store(Json::Value& val, const std::string& dir) const
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Store(val);
	}
}

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, GetTitle()); 
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	InitLayout(sizer);
	top_sizer->Add(sizer);

	top_sizer->AddSpacer(20);

	SetSizer(top_sizer);

	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
}

void ComponentPanel::OnDelete(wxCommandEvent& event)
{
	m_toolbar->OnDelChild(this);
}

void ComponentPanel::ChangeColor(uint8_t rgba[4])
{
	wxColourData data;
	data.SetColour(wxColour(rgba[0], rgba[1], rgba[2]));
	wxColourDialog dlg(m_parent, &data);

	dlg.SetTitle(wxT("Set Color"));

	wxPoint pos = wxGetMousePosition();
	pos.x -= 400;
	dlg.SetPosition(pos);

	if (dlg.ShowModal() == wxID_OK)
	{
		const wxColor& col = dlg.GetColourData().GetColour();
		rgba[0] = col.Red();
		rgba[1] = col.Green();
		rgba[2] = col.Blue();
		UpdateBtnColor();
	}
}

}