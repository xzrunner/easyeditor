#ifndef FORMATION_CONTEXT_H
#define FORMATION_CONTEXT_H



namespace formation
{
	class StagePanel;
	class ToolbarPanel;

	class Context
	{
	public:
		ee::PropertySettingPanel* property;
		ee::LibraryPanel* library;
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
