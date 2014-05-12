
#ifndef EMODELING_CONTEXT_H
#define EMODELING_CONTEXT_H

#include <drag2d.h>

namespace libmodeling { class World; }

namespace emodeling
{
	class StagePanel;
	class ToolbarPanel;

	class Context
	{
	public:
		d2d::PropertySettingPanel* property;
		d2d::LibraryPanel* library;
		StagePanel* stage;
		ToolbarPanel* toolbar;

		libmodeling::World* world;

	public:
		static Context* Instance();

	private:
		Context();
		~Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // EMODELING_CONTEXT_H
