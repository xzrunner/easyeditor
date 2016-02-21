#include "LevelEditDlg.h"

#include "dataset/LevelCfg.h"
#include "dataset/DynamicWidget.h"

#include <ee/Symbol.h>
#include <ee/ViewlistList.h>
#include <ee/EditPanelImpl.h>

namespace lr
{

LevelEditDlg::LevelEditDlg(wxWindow* parent, ee::Sprite* spr)
	: CfgEditDlg(parent, "Level", spr)
{
	InitData(spr);
	InitLayout();

	SetTitle(m_type.c_str());
}

std::string LevelEditDlg::ToString()
{
	std::string ret = "type=" + m_type + ";";
 	for (int i = 0, n = m_widgets.size(); i < n; ++i) {
 		DynamicWidget* dw = m_widgets[i];
		if (dw->GetValue() == DEFAULT_VAL) {
			continue;
		}
		if (dw->GetTag() == "sprite" || dw->IsChanged()) {
			ret += dw->GetKey() + "=" + dw->GetValue() + ";";
 		}
 	}
	return ret;
}

void LevelEditDlg::InitData(ee::Sprite* spr)
{
	if (spr->tag.find("[symbol]") != std::string::npos) {
		m_info.LoadFromString(spr->GetSymbol().tag, "symbol");
	}
	m_info.LoadFromString(spr->tag, "sprite");

	m_type = m_info.QueryValue("type");
}

void LevelEditDlg::InitLayout()
{
	if (m_type.empty()) {
		return;
	}

	int col;
	if (!LevelCfg::Instance()->QueryLayout(m_type, col, m_widgets)) {
		return;
	}

	CfgEditDlg::InitLayout(col, m_widgets);
}

}