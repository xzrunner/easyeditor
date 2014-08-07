#ifndef EPACKER_CONTEXT_H
#define EPACKER_CONTEXT_H

#include <drag2d.h>

namespace epacker
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
		int padding;
		int extrude;
		float scale;
		bool auto_arrange;
		bool premultiplied_alpha;

		Json::Value tp_meta;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // EPACKER_CONTEXT_H
