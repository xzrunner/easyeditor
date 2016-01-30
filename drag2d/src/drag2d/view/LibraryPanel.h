#ifndef _DRAG2D_LIBRARY_PANEL_H_
#define _DRAG2D_LIBRARY_PANEL_H_

#include <vector>

#include <wx/wx.h>
#include <wx/notebook.h>

#include "message/Observer.h"

namespace d2d
{

class Symbol;
class ILibraryPage;
class SymbolMgr;
class LibraryList;
class Visitor;

class LibraryPanel : public wxPanel, public Observer
{
public:
	LibraryPanel(wxWindow* parent);

	virtual void OnPageChanged(wxBookCtrlEvent& event);
	virtual void OnPageChanging(wxBookCtrlEvent& event);

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) { return true; }

	void Clear();

	void ReloadTexture() const;

	void AddPage(ILibraryPage* page, const char* name = NULL);

	Symbol* GetSymbol(int index = -1) const;

	void LoadFromSymbolMgr(const SymbolMgr& mgr);
	void LoadSymbol(Symbol* symbol);

	void Traverse(Visitor& visitor) const;

	wxWindow* GetNotebook() { return m_notebook; }

	bool AddSymbol(Symbol* symbol);

	void LoadFromConfig();
	void LoadDefaultSymbol();

	ILibraryPage* GetCurrPage() { return m_selected; }
	void SetCurrPage(int idx);
 
protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void InitLayout();

protected:
	wxNotebook* m_notebook;

	std::vector<ILibraryPage*> m_pages;

	ILibraryPage* m_selected;

}; // LibraryPanel

}

#endif // _DRAG2D_LIBRARY_PANEL_H_