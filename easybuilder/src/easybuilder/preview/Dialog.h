
#ifndef EBUILDER_PREVIEW_DIALOG_H
#define EBUILDER_PREVIEW_DIALOG_H

#include <drag2d.h>

namespace ebuilder
{
	class Scene;
	class LibraryPanel;

	namespace preview
	{
		class Dialog : public wxDialog
		{
		public:
			Dialog(wxWindow* parent, Scene* scene,
				LibraryPanel* libraryPanel);
			virtual ~Dialog();

		}; // Dialog
	}
}

#endif // EBUILDER_PREVIEW_DIALOG_H
