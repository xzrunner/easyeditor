#include "LibraryPanel.h"
#include "LayerLibraryPage.h"

#include <wx/notebook.h>

namespace emask
{

LibraryPanel::LibraryPanel(wxWindow* parent)
	: ee::LibraryPanel(parent)
{
	wxWindow* nb = GetNotebook();
	AddPage(m_base_page = new LayerLibraryPage(nb, "╣в╡Ц"));
	AddPage(m_mask_page = new LayerLibraryPage(nb, "узуж╡Ц"));
}

}