
#ifndef EBUILDER_PREVIEW_CANVAS_H
#define EBUILDER_PREVIEW_CANVAS_H



namespace ebuilder
{
	class LibraryPanel;

	namespace preview
	{
		class Panel;

		class Canvas : public ee::OrthoCanvas
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
