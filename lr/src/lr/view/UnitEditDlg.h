#ifndef _LR_UNIT_EDIT_DIALOG_H_
#define _LR_UNIT_EDIT_DIALOG_H_

#include <drag2d.h>

#include "dataset/UnitCfg.h"
#include "dataset/UnitInfo.h"

namespace lr
{

class UnitEditDlg : public d2d::BaseDialog
{
public:
	UnitEditDlg(wxWindow* parent, d2d::ISprite* spr, 
		const std::vector<std::string>& path_names);
	virtual ~UnitEditDlg();

	std::string ToString();

private:
	void InitDefault();
	void InitLayout();

private:
	d2d::ISprite* m_spr;

	UnitInfo m_info;

	const std::vector<std::string>& m_path_names;

//	std::vector<UnitWidget*> m_special_widgets;

}; // UnitEditDlg

}

#endif // _LR_UNIT_EDIT_DIALOG_H_