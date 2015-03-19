#ifndef _DRAG2D_STAGE_DROP_TARGET_H_
#define _DRAG2D_STAGE_DROP_TARGET_H_

#include <wx/dnd.h>

namespace d2d
{

class EditPanel;
class MultiSpritesImpl;
class LibraryPanel;
class ISymbol;
class Vector;

class StageDropTarget : public wxDropTarget
{
public:
	StageDropTarget(EditPanel* edit_panel, MultiSpritesImpl* sprites_impl, 
		LibraryPanel* library);

	//
	//	interface wxDropTarget
	//
	virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

	virtual bool OnDropSymbol(ISymbol* symbol, const Vector& pos) { return false; }

	void OnDropText(wxCoord x, wxCoord y, const wxString& text);
	void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	class Data : public wxDataObjectComposite
	{
	public:
		Data(StageDropTarget& drop);

		virtual bool SetData(const wxDataFormat& format, size_t len, const void *buf);

		bool OnData(wxCoord x, wxCoord y);

	private:
		StageDropTarget& m_drop;

		wxDataObjectSimple *m_data_object_last;

		wxDECLARE_NO_COPY_CLASS(Data);

	}; // Data 

private:
	EditPanel* m_edit_panel;
	MultiSpritesImpl* m_sprites_impl;
	LibraryPanel* m_library;

}; // StageDropTarget 

}

#endif // _DRAG2D_STAGE_DROP_TARGET_H_