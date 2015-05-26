#pragma once

#include <vector>

#include <wx/wx.h>
#include <wx/notebook.h>

#include "interfaces.h"

namespace d2d
{
	class ISymbol;
	class ILibraryPage;
	class SymbolMgr;
	class LibraryList;
	class IVisitor;
	class GLCanvas;

	class LibraryPanel : public wxPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

		virtual void onPageChanged(wxBookCtrlEvent& event);

		virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) { return true; }

		void clear();

		void reloadTexture() const;

		void addPage(ILibraryPage* page, const char* name = NULL);

		ISymbol* getSymbol(int index = -1) const;

		void loadFromSymbolMgr(const SymbolMgr& mgr);
		void LoadSymbol(d2d::ISymbol* symbol);

		void traverse(IVisitor& visitor) const;

		wxWindow* getNotebook() { return m_notebook; }

		void setCanvas(GLCanvas* canvas);

		bool AddSymbol(ISymbol* symbol);

		void LoadFromConfig();

		ILibraryPage* GetCurrPage() { return m_selected; }

	private:
		void initLayout();

	protected:
		wxNotebook* m_notebook;

		std::vector<ILibraryPage*> m_pages;

		ILibraryPage* m_selected;

	}; // LibraryPanel
}

