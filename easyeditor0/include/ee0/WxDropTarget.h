#pragma once

#include <wx/dnd.h>

#include <SM_Vector.h>
#include <sprite2/typedef.h>

#include <memory>

namespace ee0
{

class WxDropTarget : public wxDropTarget
{
public:
	WxDropTarget();

	//
	//	interface wxDropTarget
	//
	virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def) override;

	virtual bool OnDropSymbol(const s2::SymPtr& sym, const sm::vec2& pos) { return false; }
	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text) = 0;
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) = 0;

}; // WxDropTarget

}