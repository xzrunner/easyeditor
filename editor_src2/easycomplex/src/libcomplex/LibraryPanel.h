#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class LibraryPage;

	class LibraryPanel : public d2d::LibraryPanel
	{
	public:
		LibraryPanel(wxWindow* parent);

		//
		//ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

	}; // LibraryPanel
}

