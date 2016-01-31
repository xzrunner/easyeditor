#ifndef _EASYMAP_PASTE_SYMBOL_OP_H_
#define _EASYMAP_PASTE_SYMBOL_OP_H_



namespace emap
{

class StagePanel;

class PasteSymbolOP : public ee::PasteSymbolOP
{
public:
	PasteSymbolOP(StagePanel* stage, ee::LibraryPanel* library, 
		ee::PropertySettingPanel* property);

//	virtual bool OnMouseRightDown(int x, int y);

private:
	void ChangeCurrOP();

private:
	ee::PropertySettingPanel* m_property;

}; // PasteSymbolOP

}

#endif // _EASYMAP_PASTE_SYMBOL_OP_H_