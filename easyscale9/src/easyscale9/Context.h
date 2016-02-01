#ifndef _EASYSCALE9_CONTEXT_H_
#define _EASYSCALE9_CONTEXT_H_

namespace ee { class PropertySettingPanel; class LibraryPanel; }

namespace escale9
{

class StagePanel;
class ToolbarPanel;
class ResizeCMPT;

class Context
{
public:
	ee::PropertySettingPanel* property;
	ee::LibraryPanel* library;
	StagePanel* stage;
	ToolbarPanel* toolbar;

	bool isComposeOP;

	ResizeCMPT* resizeCMPT;

	float width, height;

public:
	static Context* Instance();

private:
	Context();

private:
	static Context* m_instance;

}; // Context

}

#endif // _EASYSCALE9_CONTEXT_H_