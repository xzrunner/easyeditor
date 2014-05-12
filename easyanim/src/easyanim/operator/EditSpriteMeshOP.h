#pragma once

#include "EditMeshUtility.h"

#include <drag2d.h>

namespace eanim
{
	class Mesh;
	struct MeshNode;
	class MeshTri;

	class EditSpriteMeshOP : public d2d::ZoomViewOP, private EditMeshUtility
	{
	public:
		EditSpriteMeshOP(d2d::EditPanel* editPanel, Mesh* mesh);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		MeshTri* selectTriByPos(const d2d::Vector& pos) const;

	private:
		Mesh* m_mesh;

		MeshNode* m_selectedNode;
		MeshTri* m_selectedTri;

		d2d::Vector m_lastPos;

	}; // EditSpriteMeshOP
}

