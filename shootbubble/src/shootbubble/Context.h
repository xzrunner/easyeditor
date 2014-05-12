#ifndef SHOOTBUBBLE_CONTEXT_H
#define SHOOTBUBBLE_CONTEXT_H

#include <drag2d.h>

namespace shootbubble
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

		int row, col;
		int edge;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // SHOOTBUBBLE_CONTEXT_H
