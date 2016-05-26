#ifndef _EASYMASK_LAYER_LIBRARY_PAGE_H_
#define _EASYMASK_LAYER_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace emask
{

class LayerLibraryPage : public ee::LibraryPage
{
public:
	LayerLibraryPage(wxWindow* parent, const std::string& name, bool only_img);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

private:
	bool m_only_img;

}; // LayerLibraryPage

}

#endif // _EASYMASK_LAYER_LIBRARY_PAGE_H_