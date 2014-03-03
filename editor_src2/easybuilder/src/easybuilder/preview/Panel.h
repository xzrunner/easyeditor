
#ifndef EBUILDER_PREVIEW_PANEL_H
#define EBUILDER_PREVIEW_PANEL_H

#include <drag2d.h>

namespace ebuilder
{
	class LibraryPanel;

	namespace preview
	{
		class Canvas;
		class Scene;

		class Panel : public d2d::EditPanel
		{
		public:
			Panel(wxWindow* parent, wxTopLevelWindow* frame,
				LibraryPanel* libraryPanel);
			virtual ~Panel();

			Canvas* getCanvas() {
				return m_canvas;
			}

			Scene* getScene() const;
			void setScene(Scene* scene);

		private:
			class MouseActionOP : public d2d::AbstractEditOP
			{
			public:
				MouseActionOP(Panel* panel);

				virtual bool onMouseLeftDown(int x, int y);
				virtual bool onMouseLeftUp(int x, int y);

			private:
				Panel* m_panel;

			}; // MouseActionOP

		private:
			Canvas* m_canvas;

			Scene* m_scene;

		}; // Panel
	}
}

#endif // EBUILDER_PREVIEW_PANEL_H
