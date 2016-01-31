#pragma once



namespace escale9
{
	class StagePanel;
	class ToolbarPanel;
	class ResizeCMPT;

	class Context
	{
	public:
		ee::PropertySettingPanel* property;
		ee::LibraryPanel* library;
		StagePanel* stage;
		ToolbarPanel* toolbar;

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

