#pragma once

#include "SelectNodesOP.h"

namespace d2d
{
	class EditNodesOP : public SelectNodesOP
	{
	public:
		EditNodesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl);

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		void simplify(float threshold);
		void smooth(float samplingWidth);
		void updateModified();

	private:
		struct Modified
		{
			SelectNodesOP::ChainSelectedNodes* src;
			std::vector<Vector> dst;
		};

	private:
		Vector m_lastPos;

		std::vector<Modified> m_buffer;

	}; // EditNodesOP
}

