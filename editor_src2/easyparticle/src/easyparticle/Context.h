
#ifndef EPARTICLE_CONTEXT_H
#define EPARTICLE_CONTEXT_H

#include <drag2d.h>

namespace eparticle
{
	namespace coco { class StagePanel; class ToolbarPanel; }
	class ToolbarPanel;

	class Context
	{
	public:
		coco::StagePanel* stage;
		coco::ToolbarPanel* toolbar;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // EPARTICLE_CONTEXT_H
