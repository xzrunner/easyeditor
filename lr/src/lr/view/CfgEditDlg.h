#ifndef _LR_CFG_EDIT_DIALOG_H_
#define _LR_CFG_EDIT_DIALOG_H_

#include <drag2d.h>

#include "dataset/DynamicInfo.h"

namespace lr
{

class DynamicWidget;

class CfgEditDlg : public d2d::BaseDialog
{
public:
	CfgEditDlg(wxWindow* parent, const char* name, d2d::ISprite* spr);
	
protected:
	void InitLayout(int col, const std::vector<DynamicWidget*>& widgets);

protected:
	DynamicInfo m_info;

private:
	d2d::ISprite* m_spr;

}; // CfgEditDlg

}

#endif // _LR_CFG_EDIT_DIALOG_H_