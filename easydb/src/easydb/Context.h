#ifndef _EASYDB_CONTEXT_H_
#define _EASYDB_CONTEXT_H_

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

#endif // _EASYDB_CONTEXT_H_
