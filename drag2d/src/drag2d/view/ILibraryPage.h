#pragma once

#include <wx/wx.h>

#include "interfaces.h"

namespace d2d
{
	class ListItem;
	class ISymbol;
	class LibraryList;
	class GLCanvas;

	class ILibraryPage : public wxWindow
	{
	public:
		ILibraryPage(wxWindow* parent, const wxString& name, bool isStatic = false);

		virtual bool isHandleSymbol(ISymbol* symbol) const = 0;

		virtual void clear();

		void traverse(IVisitor& visitor) const;

		const wxString& getName() const { return m_name; }

		ListItem* getItem(int index = -1) const;
		ISymbol* getSymbol(int index = -1) const;

		void reloadTexture() const;

		LibraryList* getList() { return m_list; }

		void setCanvas(GLCanvas* canvas) {
			m_canvas = canvas;
		}

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	protected:
		void initButtons(wxSizer* sizer);

	protected:
		wxString m_name;

		bool m_isStatic;

		wxButton *m_btnAdd, *m_btnDel;

		LibraryList* m_list;

		GLCanvas* m_canvas;

	}; // ILibraryPage
}

