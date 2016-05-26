#ifndef _EASYMASK_LIBRARY_PANEL_H_
#define _EASYMASK_LIBRARY_PANEL_H_

#include <ee/LibraryPanel.h>

namespace emask
{

class LibraryPanel : public ee::LibraryPanel
{
public:
	LibraryPanel(wxWindow* parent);

	bool IsCurrBase() const { return GetCurrPage() == m_base_page; }

	ee::LibraryPage* GetPage(bool base) {
		return base ? m_base_page : m_mask_page;
	}

private:
	ee::LibraryPage *m_base_page, *m_mask_page;

}; // LibraryPanel

}

#endif // _EASYMASK_LIBRARY_PANEL_H_