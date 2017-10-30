#ifndef _LR_LEVEL_EDIT_DIALOG_H_
#define _LR_LEVEL_EDIT_DIALOG_H_

#include <ee/CfgEditDlg.h>

namespace lr
{

class LevelEditDlg : public ee::CfgEditDlg
{
public:
	LevelEditDlg(wxWindow* parent, const ee::SprPtr& spr);

	std::string ToString();

private:
	void InitData(const ee::SprPtr& spr);
	void InitLayout();

private:
	std::string m_type;

	std::vector<ee::DynamicWidget*> m_widgets;

}; // LevelEditDlg

}

#endif // _LR_LEVEL_EDIT_DIALOG_H_