#pragma once

#include <easy9patch.h>

namespace e9patch
{
	class ResizeOP : public lib9patch::ResizeOP
	{
	public:
		ResizeOP(d2d::EditPanel* editPanel);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onActive();

		virtual bool onDraw() const;

	}; // ResizeOP
}

