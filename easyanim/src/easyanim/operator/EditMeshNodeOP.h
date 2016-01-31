#pragma once

#include "EditMeshUtility.h"



namespace eanim
{
	class Mesh;
	struct MeshNode;

	class EditMeshNodeOP : public ee::ZoomViewOP, private EditMeshUtility
	{
	public:
		EditMeshNodeOP(ee::EditPanel* editPanel, Mesh* mesh);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		
		virtual bool onDraw() const;
		virtual bool clear();

	private:
		Mesh* m_mesh;

		ee::Vector m_firstPos;
		MeshNode* m_selected;

	}; // EditMeshNodeOP
}

