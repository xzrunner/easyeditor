#ifndef _EASYMAP_PASTE_SYMBOL_OP_H_
#define _EASYMAP_PASTE_SYMBOL_OP_H_

#include <drag2d.h>

namespace emap
{

class StagePanel;

class PasteSymbolOP : public d2d::PasteSymbolOP
{
public:
	PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library, 
		d2d::PropertySettingPanel* property);

//	virtual bool onMouseRightDown(int x, int y);

private:
	void ChangeCurrOP();

private:
	d2d::PropertySettingPanel* m_property;

}; // PasteSymbolOP

}

#endif // _EASYMAP_PASTE_SYMBOL_OP_H_