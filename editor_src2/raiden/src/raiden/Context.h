#ifndef RAIDEN_CONTEXT_H
#define RAIDEN_CONTEXT_H

#include <drag2d.h>

namespace raiden
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

		d2d::LayersMgrWidget* layers;

		int speed;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // RAIDEN_CONTEXT_H
