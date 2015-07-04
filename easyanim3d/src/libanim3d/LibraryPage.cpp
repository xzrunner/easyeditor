#include "LibraryPage.h"
#include "config.h"
#include "Symbol.h"

#include <easy3d.h>

namespace eanim3d
{

LibraryPage::LibraryPage(wxWindow* parent)
	: d2d::ILibraryPage(parent, wxT("3D"))
{
	InitLayout();
	m_list->setFileter(FILE_TAG);
}

bool LibraryPage::IsHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<Symbol*>(symbol) != NULL;
}

void LibraryPage::OnAddPress(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("choose model files"));
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			e3d::ModelParametric* model = new e3d::ModelParametric();
			e3d::AssimpHelper loader;
			e3d::AABB aabb;
			loader.LoadFile(filenames[i], *model, aabb);
			Symbol* symbol = new Symbol();
			symbol->SetModel(model);

			wxString filepath = FILE_TAG;
			filepath += ".json";
			symbol->SetFilepath(filepath);
			symbol->SetAABB(aabb);

			AddItem(symbol);
		}
	}
}

}