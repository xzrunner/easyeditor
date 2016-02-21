#ifndef _LR_CFG_EDIT_DIALOG_H_
#define _LR_CFG_EDIT_DIALOG_H_

#include "dataset/DynamicInfo.h"

#include <ee/BaseDialog.h>
#include <ee/Sprite.h>

namespace lr
{

class DynamicWidget;

class CfgEditDlg : public ee::BaseDialog
{
public:
	CfgEditDlg(wxWindow* parent, const char* name, ee::Sprite* spr);
	
protected:
	void InitLayout(int col, const std::vector<DynamicWidget*>& widgets);

protected:
	DynamicInfo m_info;

private:
	ee::Sprite* m_spr;

}; // CfgEditDlg

}

#endif // _LR_CFG_EDIT_DIALOG_H_