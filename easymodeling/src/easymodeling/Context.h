#ifndef _EASYMODELING_CONTEXT_H_
#define _EASYMODELING_CONTEXT_H_

namespace ee { class PropertySettingPanel; class LibraryPanel; }

namespace emodeling
{

class World;
class StagePanel;
class ToolbarPanel;

class Context
{
public:
	ee::PropertySettingPanel* property;
	ee::LibraryPanel* library;
	StagePanel* stage;
	ToolbarPanel* toolbar;

	World* world;

public:
	static Context* Instance();

private:
	Context();
	~Context();

private:
	static Context* m_instance;

}; // Context

}

#endif // _EASYMODELING_CONTEXT_H_
