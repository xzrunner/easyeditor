#ifndef _EASYTEXPACKER_CONTEXT_H_
#define _EASYTEXPACKER_CONTEXT_H_



namespace etexpacker
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
