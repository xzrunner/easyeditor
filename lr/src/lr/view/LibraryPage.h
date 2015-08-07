#ifndef _LR_LIBRARY_PAGE_H_
#define _LR_LIBRARY_PAGE_H_

#include <drag2d.h>

#include "dataset/layer_type.h"

namespace lr
{

class LibraryPanel;
class Layer;

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(LibraryPanel* library, const std::string& name, 
		LayerType type);
	virtual ~LibraryPage();

	//
	//	interface d2d::ILibraryPage
	//
	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

	void UpdateStatusFromLayer();

	Layer* GetLayer() { return m_layer; }
	void SetLayer(Layer* layer);

	void AddEditOP(d2d::AbstractEditOP* editop);
	d2d::AbstractEditOP* GetNextEditOP();

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

	int m_curr_op_idx;
	std::vector<d2d::AbstractEditOP*> m_editops;

}; // LibraryPage 

}

#endif // _LR_LIBRARY_PAGE_H_