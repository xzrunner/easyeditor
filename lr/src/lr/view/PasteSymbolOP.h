#ifndef _LR_PASTE_SYMBOL_OP_H_
#define _LR_PASTE_SYMBOL_OP_H_

#include <drag2d.h>

namespace lr
{

class StagePanel;

class PasteSymbolOP : public d2d::PasteSymbolOP
{
public:
	PasteSymbolOP(StagePanel* stage, d2d::LibraryPanel* library, 
		d2d::PropertySettingPanel* property);

private:
	void ChangeCurrOP();

private:
	d2d::PropertySettingPanel* m_property;

}; // PasteSymbolOP

}

#endif // _LR_PASTE_SYMBOL_OP_H_