#ifndef _EASYEDITOR_CFG_EDIT_DIALOG_H_
#define _EASYEDITOR_CFG_EDIT_DIALOG_H_

#include "BaseDialog.h"
#include "DynamicInfo.h"
#include "Sprite.h"

#include <vector>

namespace ee
{

class DynamicWidget;

class CfgEditDlg : public BaseDialog
{
public:
	CfgEditDlg(wxWindow* parent, const char* name, const SprPtr& spr);
	
protected:
	void InitLayout(int col, const std::vector<DynamicWidget*>& widgets);

protected:
	DynamicInfo m_info;

private:
	SprPtr m_spr;

}; // CfgEditDlg

}

#endif // _EASYEDITOR_CFG_EDIT_DIALOG_H_