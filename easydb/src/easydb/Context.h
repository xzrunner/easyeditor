#ifndef EDB_CONTEXT_H
#define EDB_CONTEXT_H



namespace edb
{
	class TreeCtrl;
	class StagePanel;

	class Context
	{
	public:
		TreeCtrl* tree;

		StagePanel* stage;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // EDB_CONTEXT_H
