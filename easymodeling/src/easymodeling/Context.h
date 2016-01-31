
#ifndef EMODELING_CONTEXT_H
#define EMODELING_CONTEXT_H



namespace libmodeling { class World; }

namespace emodeling
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

		libmodeling::World* world;

	public:
		static Context* Instance();

	private:
		Context();
		~Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // EMODELING_CONTEXT_H
