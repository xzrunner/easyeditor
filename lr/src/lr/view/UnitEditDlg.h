#ifndef _LR_UNIT_EDIT_DIALOG_H_
#define _LR_UNIT_EDIT_DIALOG_H_

#include "CfgEditDlg.h"

namespace lr
{

class UnitEditDlg : public CfgEditDlg
{
public:
	UnitEditDlg(wxWindow* parent, d2d::Sprite* spr, 
		const std::vector<std::string>& path_names);
	virtual ~UnitEditDlg();

	std::string ToString();

private:
	void InitDefault();
	void InitLayout();

private:
	const std::vector<std::string>& m_path_names;

//	std::vector<DynamicWidget*> m_special_widgets;

}; // UnitEditDlg

}

#endif // _LR_UNIT_EDIT_DIALOG_H_