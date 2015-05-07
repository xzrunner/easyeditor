#ifndef _LR_LIBRARY_PAGE_H_
#define _LR_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace lr
{

class StagePanel;
class Layer;

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent, const std::string& name, 
		d2d::AbstractEditOP* editop);
	virtual ~LibraryPage();

	virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	Layer* GetLayer() { return m_layer; }
	void SetLayer(Layer* layer);

	d2d::AbstractEditOP* GetEditOP() { return m_editop; }

protected:
	virtual void InitLayoutExtend(wxSizer* sizer);

	virtual void onAddPress(wxCommandEvent& event);

private:
	void OnChangeVisible(wxCommandEvent& event);
	void OnChangeEditable(wxCommandEvent& event);

	void VisibleAllSprites(bool visible);
	void EditableAllSprites(bool visible);

private:
	Layer* m_layer;

	d2d::AbstractEditOP* m_editop;

}; // LibraryPage 

}

#endif // _LR_LIBRARY_PAGE_H_