#ifndef _EASYEDITOR_PASTE_SYMBOL_CAPTURE_OP_H_
#define _EASYEDITOR_PASTE_SYMBOL_CAPTURE_OP_H_

#include "PasteSymbolOP.h"
#include "PasteSymbolOffsetCMPT.h"

namespace ee
{

class PasteSymbolCaptureOP;
class LibraryPanel;

class PasteSymbolCaptureOP : public PasteSymbolOP
{
public:
	PasteSymbolCaptureOP(wxWindow* wnd, EditPanelImpl* stage, LibraryPanel* library, 
		PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* cmpt);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);

	virtual bool Clear();

private:
	PasteSymbolOffsetCMPT<PasteSymbolCaptureOP>* m_cmpt;

	bool m_bCaptured;
	sm::vec2 m_last_pos;
	bool m_last_pos_valid;

}; // PasteSymbolCaptureOP

}

#endif // _EASYEDITOR_PASTE_SYMBOL_CAPTURE_OP_H_