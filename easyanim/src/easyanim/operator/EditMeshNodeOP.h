#pragma once

#include "EditMeshUtility.h"

#include <drag2d.h>

namespace eanim
{
	class Mesh;
	struct MeshNode;

	class EditMeshNodeOP : public d2d::ZoomViewOP, private EditMeshUtility
	{
	public:
		EditMeshNodeOP(d2d::EditPanel* editPanel, Mesh* mesh);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		
		virtual bool onDraw() const;
		virtual bool clear();

	private:
		Mesh* m_mesh;

		d2d::Vector m_firstPos;
		MeshNode* m_selected;

	}; // EditMeshNodeOP
}

