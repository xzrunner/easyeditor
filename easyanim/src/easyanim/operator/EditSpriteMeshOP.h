#pragma once

#include "EditMeshUtility.h"



namespace eanim
{
	class Mesh;
	struct MeshNode;
	class MeshTri;

	class EditSpriteMeshOP : public ee::ZoomViewOP, private EditMeshUtility
	{
	public:
		EditSpriteMeshOP(ee::EditPanel* editPanel, Mesh* mesh);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		MeshTri* selectTriByPos(const ee::Vector& pos) const;

	private:
		Mesh* m_mesh;

		MeshNode* m_selectedNode;
		MeshTri* m_selectedTri;

		ee::Vector m_lastPos;

	}; // EditSpriteMeshOP
}

