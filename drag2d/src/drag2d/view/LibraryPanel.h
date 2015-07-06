#ifndef _DRAG2D_LIBRARY_PANEL_H_
#define _DRAG2D_LIBRARY_PANEL_H_

#include <vector>

#include <wx/wx.h>
#include <wx/notebook.h>

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

	virtual void OnPageChanged(wxBookCtrlEvent& event);
	virtual void OnPageChanging(wxBookCtrlEvent& event);

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) { return true; }

	void Clear();

	void ReloadTexture() const;

	void AddPage(ILibraryPage* page, const char* name = NULL);

	ISymbol* GetSymbol(int index = -1) const;

	void LoadFromSymbolMgr(const SymbolMgr& mgr);
	void LoadSymbol(d2d::ISymbol* symbol);

	void Traverse(IVisitor& visitor) const;

	wxWindow* GetNotebook() { return m_notebook; }

	void SetCanvas(GLCanvas* canvas);

	bool AddSymbol(ISymbol* symbol);

	void LoadFromConfig();

	ILibraryPage* GetCurrPage() { return m_selected; }
	void SetCurrPage(int idx);

private:
	void InitLayout();

protected:
	wxNotebook* m_notebook;

	std::vector<ILibraryPage*> m_pages;

	ILibraryPage* m_selected;

}; // LibraryPanel

}

#endif // _DRAG2D_LIBRARY_PANEL_H_