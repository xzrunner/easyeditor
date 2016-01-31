#pragma once


namespace eanim
{
	class Mesh;
	struct MeshNode;

	class EditMeshUtility
	{
	public:
		EditMeshUtility(Mesh* mesh);

	protected:
		static const float NODE_RADIUS;

	protected:
		void draw() const;

		MeshNode* selectNodeByPos(const ee::Vector& pos) const;

	protected:
		Mesh* m_mesh;

	}; // EditMeshUtility
}

