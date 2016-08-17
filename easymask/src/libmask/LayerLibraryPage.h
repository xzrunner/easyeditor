#ifndef _EASYMASK_LAYER_LIBRARY_PAGE_H_
#define _EASYMASK_LAYER_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace emask
{

class LayerLibraryPage : public ee::LibraryPage
{
public:
	LayerLibraryPage(wxWindow* parent, const std::string& name);

	virtual bool IsHandleSymbol(ee::Symbol* sym) const { return true; } 

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LayerLibraryPage

}

#endif // _EASYMASK_LAYER_LIBRARY_PAGE_H_