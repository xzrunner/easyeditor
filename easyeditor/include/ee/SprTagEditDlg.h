#ifndef _EASYEDITOR_SPR_TAG_EDIT_DLG_H_
#define _EASYEDITOR_SPR_TAG_EDIT_DLG_H_

#include "CfgEditDlg.h"

namespace ee
{

class SprTagEditDlg : public CfgEditDlg
{
public:
	SprTagEditDlg(wxWindow* parent, ee::Sprite* spr);

	std::string ToString();

private:
	void InitDefault();
	void InitLayout();

}; // SprTagEditDlg

}

#endif // _EASYEDITOR_SPR_TAG_EDIT_DLG_H_