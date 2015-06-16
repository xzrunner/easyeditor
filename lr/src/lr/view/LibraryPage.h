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

	//
	//	interface d2d::ILibraryPage
	//
	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

	void UpdateStatusFromLayer();

	Layer* GetLayer() { return m_layer; }
	void SetLayer(Layer* layer);

	d2d::AbstractEditOP* GetExtenOP() { return m_exten_op; }

protected:
	virtual void InitLayoutExtend(wxSizer* sizer);

	virtual void OnAddPress(wxCommandEvent& event);

private:
	void OnChangeVisible(wxCommandEvent& event);
	void OnChangeEditable(wxCommandEvent& event);

	void VisibleAllSprites(bool visible);
	void EditableAllSprites(bool visible);

private:
	Layer* m_layer;

	wxCheckBox* m_visible_ctrl;
	wxCheckBox* m_editable_ctrl;

	d2d::AbstractEditOP* m_exten_op;

}; // LibraryPage 

}

#endif // _LR_LIBRARY_PAGE_H_