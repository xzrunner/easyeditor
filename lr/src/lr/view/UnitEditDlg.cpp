#include "UnitEditDlg.h"

#include "dataset/UnitCfg.h"

#include <ee/DynamicWidget.h>
#include <ee/Sprite.h>

namespace lr
{

UnitEditDlg::UnitEditDlg(wxWindow* parent, ee::Sprite* spr,
						 const std::vector<std::string>& path_names)
	: CfgEditDlg(parent, "Unit", spr)
	, m_path_names(path_names)
{	
	m_info.LoadFromString(spr->GetTag());

	InitDefault();
	InitLayout();
}

UnitEditDlg::~UnitEditDlg()
{
//	for_each(m_special_widgets.begin(), m_special_widgets.end(), DeletePointerFunctor<ee::DynamicWidget>());
}

std::string UnitEditDlg::ToString()
{
	UnitCfg* cfg = UnitCfg::Instance();
	for (int i = 0, n = cfg->widgets.size(); i < n; ++i) {
		ee::DynamicWidget* widget = cfg->widgets[i];
		m_info.SetValue(widget->GetKey(), widget->GetValue(), widget->GetValue() == ee::DynamicWidget::DEFAULT_VAL);
	}
// 	for (int i = 0, n = m_special_widgets.size(); i < n; ++i) {
// 		ee::DynamicWidget* widget = m_special_widgets[i];
// 		m_info.SetValue(widget->GetKey(), widget->GetValue());
// 	}
	 
	return m_info.ToString();
}

void UnitEditDlg::InitDefault()
{
// 	// patrol
// 	ChoiceWidget* patrol = new ChoiceWidget;
// 	patrol->SpecialLoad("Ñ²Âß", "patrol", m_path_names, m_info.QueryValue("patrol"));
// 	m_special_widgets.push_back(patrol);
}

void UnitEditDlg::InitLayout()
{
	UnitCfg* cfg = UnitCfg::Instance();
	CfgEditDlg::InitLayout(cfg->col, cfg->widgets);
}

}