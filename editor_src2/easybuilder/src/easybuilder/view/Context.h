
#ifndef EBUILDER_CONTEXT_H
#define EBUILDER_CONTEXT_H

namespace ebuilder
{
	class LibraryPanel;
	class PropertySettingPanel;
	class BackstagePanel;
	class StagePanel;

	class Context
	{
	public:
		LibraryPanel* library;
		PropertySettingPanel* property;
		BackstagePanel* backstage;
		StagePanel* stage;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // EBUILDER_CONTEXT_H
