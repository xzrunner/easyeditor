#ifndef SHOOTBUBBLE_CONTEXT_H
#define SHOOTBUBBLE_CONTEXT_H



namespace shootbubble
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
