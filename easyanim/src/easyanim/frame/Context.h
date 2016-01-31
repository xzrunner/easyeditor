#pragma once



namespace eanim
{
	class LibraryPanel;
	class TimeLinePanel;
	class StagePanel;

	class Context
	{
	public:
		LibraryPanel* library;
		TimeLinePanel* timeline;
		StagePanel* stage;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

