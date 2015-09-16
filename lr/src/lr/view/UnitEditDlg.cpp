#include "UnitEditDlg.h"

#include "dataset/UnitWidget.h"

namespace lr
{

UnitEditDlg::UnitEditDlg(wxWindow* parent, d2d::ISprite* spr,
						 const std::vector<std::string>& path_names)
	: BaseDialog(parent, wxT("Unit"))
	, m_spr(spr)
	, m_info(spr->tag)
	, m_path_names(path_names)
{
	InitDefault();
	InitLayout();
}

UnitEditDlg::~UnitEditDlg()
{
	for_each(m_special_widgets.begin(), m_special_widgets.end(), DeletePointerFunctor<UnitWidget>());
}

std::string UnitEditDlg::ToString()
{
	UnitCfg* cfg = UnitCfg::Instance();
	for (int i = 0, n = cfg->widgets.size(); i < n; ++i) {
		UnitWidget* widget = cfg->widgets[i];
		m_info.SetValue(widget->GetKey(), widget->GetValue());
	}
	for (int i = 0, n = m_special_widgets.size(); i < n; ++i) {
		UnitWidget* widget = m_special_widgets[i];
		m_info.SetValue(widget->GetKey(), widget->GetValue());
	}

	m_spr->name = m_info.QueryValue("name");
	 
	return m_info.ToString();
}

void UnitEditDlg::InitDefault()
{
	if (!m_spr->name.empty()) {
		m_info.SetValue("name", m_spr->name);
	}

	// patrol
	UnitChoiceWidget* patrol = new UnitChoiceWidget;
	patrol->SpecialLoad("Ñ²Âß", "patrol", m_path_names, m_info.QueryValue("patrol"));
	m_special_widgets.push_back(patrol);
}

void UnitEditDlg::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	UnitCfg* cfg = UnitCfg::Instance();
	for (int i = 0, n = cfg->widgets.size(); i < n; ++i) {
		cfg->widgets[i]->InitLayout(this, sizer, m_info);
	}
	for (int i = 0, n = m_special_widgets.size(); i < n; ++i) {
		m_special_widgets[i]->InitLayout(this, sizer, m_info);		
	}

	sizer->AddSpacer(20);
	{
		wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
		sz->Add(new wxButton(this, wxID_OK), 0, wxALL, 5);
		sz->Add(new wxButton(this, wxID_CANCEL), 0, wxALL, 5);
		sizer->Add(sz, 0, wxCENTER);
	}
	sizer->AddSpacer(20);

	SetSizer(sizer);
	sizer->Fit(this);
}

}