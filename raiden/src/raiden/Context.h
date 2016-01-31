#ifndef RAIDEN_CONTEXT_H
#define RAIDEN_CONTEXT_H



namespace raiden
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

		ee::LayersMgrWidget* layers;

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
