#ifndef FORMATION_CONTEXT_H
#define FORMATION_CONTEXT_H

#include <drag2d.h>

namespace formation
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

		int width, height;
		int time;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // FORMATION_CONTEXT_H
