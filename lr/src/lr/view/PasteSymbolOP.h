#ifndef _LR_PASTE_SYMBOL_OP_H_
#define _LR_PASTE_SYMBOL_OP_H_

#include <ee/PasteSymbolOP.h>

namespace ee { class PropertySettingPanel; }

namespace lr
{

class StagePanel;

class PasteSymbolOP : public ee::PasteSymbolOP
{
public:
	PasteSymbolOP(StagePanel* stage, ee::LibraryPanel* library, 
		ee::PropertySettingPanel* property);

private:
	void ChangeCurrOP();

private:
	ee::PropertySettingPanel* m_property;

}; // PasteSymbolOP

}

#endif // _LR_PASTE_SYMBOL_OP_H_