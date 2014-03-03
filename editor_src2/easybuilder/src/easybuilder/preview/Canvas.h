
#ifndef EBUILDER_PREVIEW_CANVAS_H
#define EBUILDER_PREVIEW_CANVAS_H

#include <drag2d.h>

namespace ebuilder
{
	class LibraryPanel;

	namespace preview
	{
		class Panel;

		class Canvas : public d2d::OrthoCanvas
		{
		public:
			Canvas(Panel* panel, LibraryPanel* libraryPanel);
			virtual ~Canvas();

		protected:
			virtual void initGL();
			virtual void onDraw();

		private:
			LibraryPanel* m_libraryPanel;

		}; // Canvas
	}
}

#endif // EBUILDER_PREVIEW_CANVAS_H
