#ifndef _DRAG2D_COMBINED_DROP_TARGET_H_
#define _DRAG2D_COMBINED_DROP_TARGET_H_

#include <wx/dnd.h>

namespace d2d
{

class ISymbol;
class Vector;

class CombinedDropTarget : public wxDropTarget
{
public:
	CombinedDropTarget(wxWindow* parent);
	virtual ~CombinedDropTarget() {}
 
	//
	//	interface wxDropTarget
	//
	virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

	virtual bool OnDropSymbol(ISymbol* symbol, const Vector& pos) { return false; }
	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) = 0;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) = 0;

private:
	wxWindow* m_parent;

}; // CombinedDropTarget 

}

#endif // _DRAG2D_COMBINED_DROP_TARGET_H_