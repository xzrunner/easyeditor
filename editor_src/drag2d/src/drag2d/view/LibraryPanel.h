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

	class LibraryPanel : public wxPanel, public ISerializable
	{
	public:
		LibraryPanel(wxWindow* parent);

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual void onPageChanged(wxBookCtrlEvent& event);

		void clear();

		void reloadTexture() const;

		void addPage(ILibraryPage* page);

		ISymbol* getSymbol(int index = -1) const;

		void loadFromSymbolMgr(const SymbolMgr& mgr);

		void traverse(IVisitor& visitor) const;

		wxWindow* getNotebook() { return m_notebook; }

	private:
		void initLayout();

	protected:
		wxNotebook* m_notebook;

		std::vector<ILibraryPage*> m_pages;

		ILibraryPage* m_selected;

	}; // LibraryPanel
}

