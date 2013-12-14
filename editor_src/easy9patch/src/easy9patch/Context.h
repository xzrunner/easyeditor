#pragma once

#include <drag2d.h>

namespace e9patch
{
	class StagePanel;
	class ToolbarPanel;
	class ResizeCMPT;

	class Context
	{
	public:
		d2d::PropertySettingPanel* property;
		d2d::LibraryPanel* library;
		StagePanel* stage;
		ToolbarPanel* toolbar;

		static const int EDGE = 100;

		bool isComposeOP;

		ResizeCMPT* resizeCMPT;

		float width, height;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

