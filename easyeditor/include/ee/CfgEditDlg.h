#ifndef _EASYEDITOR_CFG_EDIT_DIALOG_H_
#define _EASYEDITOR_CFG_EDIT_DIALOG_H_

#include "BaseDialog.h"
#include "DynamicInfo.h"

#include <vector>

namespace ee
{

class Sprite;
class DynamicWidget;

class CfgEditDlg : public BaseDialog
{
public:
	CfgEditDlg(wxWindow* parent, const char* name, Sprite* spr);
	
protected:
	void InitLayout(int col, const std::vector<DynamicWidget*>& widgets);

protected:
	DynamicInfo m_info;

private:
	Sprite* m_spr;

}; // CfgEditDlg

}

#endif // _EASYEDITOR_CFG_EDIT_DIALOG_H_