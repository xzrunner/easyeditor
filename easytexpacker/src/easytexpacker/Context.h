#ifndef _EASYTEXPACKER_CONTEXT_H_
#define _EASYTEXPACKER_CONTEXT_H_

#include <drag2d.h>

namespace etp
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

#endif // _EASYTEXPACKER_CONTEXT_H_
