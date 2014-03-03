
#ifndef ESHAPE_CONTEXT_H
#define ESHAPE_CONTEXT_H

#include <drag2d.h>

namespace eshape
{
	class LibraryPanel;
	class StagePanel;
	class ToolbarPanel;
	class LibraryItem;

	class Context
	{
	public:
		d2d::PropertySettingPanel* property;
		LibraryPanel* library;
		StagePanel* stage;
		ToolbarPanel* toolbar;

	public:
		static Context* Instance();

		void changeCurrItem(LibraryItem* item);

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // ESHAPE_CONTEXT_H
