#pragma once

#include <drag2d.h>

namespace eanim
{
	class LibrarySymbolList : public d2d::LibraryList
	{
	public:
		LibrarySymbolList(wxWindow* parent);

		//
		// d2d::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

	protected:
		virtual void onListDoubleClicked(wxCommandEvent& event);

	}; // LibrarySymbolList
}

