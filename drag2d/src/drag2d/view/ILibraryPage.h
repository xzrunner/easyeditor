#ifndef _DRAG2D_I_LIBRARY_PAGE_H_
#define _DRAG2D_I_LIBRARY_PAGE_H_

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

	virtual bool IsHandleSymbol(ISymbol* symbol) const = 0;

	virtual void Clear();

	virtual void LoadFromConfig() {}

	virtual void OnActive() {}

	void Traverse(IVisitor& visitor) const;

	const wxString& GetPageName() const { return m_name; }

	ListItem* GetItem(int index = -1) const;
	ISymbol* GetSymbol(int index = -1) const;

	void ReloadTexture() const;

	LibraryList* GetList() { return m_list; }

	void SetCanvas(GLCanvas* canvas) {
		m_canvas = canvas;
	}

	void AddItem(ListItem* item);

protected:
	virtual void InitLayout(bool draggable = true);
	virtual void InitLayoutExtend(wxSizer* sizer) {}

	virtual void OnAddPress(wxCommandEvent& event);
	virtual void OnDelPress(wxCommandEvent& event);

protected:
	void InitButtons(wxSizer* sizer);

	void LoadFromConfig(const std::string& key);

protected:
	wxString m_name;

	bool m_is_static;

	wxButton *m_btn_add, *m_btn_del;

	LibraryList* m_list;

	GLCanvas* m_canvas;

}; // ILibraryPage

}

#endif // _DRAG2D_I_LIBRARY_PAGE_H_