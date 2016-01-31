#include "EditSpriteMeshOP.h"
#include "dataset/Mesh.h"

namespace eanim
{

EditSpriteMeshOP::EditSpriteMeshOP(ee::EditPanel* editPanel, Mesh* mesh)
	: ee::ZoomViewOP(editPanel, false), EditMeshUtility(mesh)
{
	m_mesh = mesh;
	m_selectedNode = NULL;	
	m_selectedTri = NULL;
}

bool EditSpriteMeshOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	m_selectedNode = selectNodeByPos(pos);

	if (!m_selectedNode && !m_mesh->isMerged())
	{
		m_selectedTri = selectTriByPos(pos);
		if (m_selectedTri) m_lastPos = pos;
	}

	return false;
}

bool EditSpriteMeshOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_selectedNode = NULL;
	m_selectedTri = NULL;

	return false;
}

bool EditSpriteMeshOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_selectedNode)
	{
		m_selectedNode->projCoords = m_editPanel->transPosScreenToProject(x, y);
		m_editPanel->Refresh();
	}
	else if (m_selectedTri)
	{
		ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		std::vector<MeshNode*> nodes;
		m_selectedTri->getNodes(nodes);
		for (size_t i = 0; i < 3; ++i)
			nodes[i]->projCoords += pos - m_lastPos;
		m_lastPos = pos;
		m_editPanel->Refresh();
	}

	return false;
}

bool EditSpriteMeshOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	draw();

	return false;
}

bool EditSpriteMeshOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_selectedNode = NULL;	
	m_selectedTri = NULL;
	m_lastPos.setInvalid();

	return false;
}

MeshTri* EditSpriteMeshOP::selectTriByPos(const ee::Vector& pos) const
{
	const std::vector<MeshTri*>& tris = m_mesh->getAllTris();
	for (size_t i = 0, n = tris.size(); i < n; ++i)
	{
		std::vector<MeshNode*> nodes;
		tris[i]->getNodes(nodes);
		std::vector<ee::Vector> bound;
		for (size_t j = 0; j < 3; ++j)
			bound.push_back(nodes[j]->projCoords);

		if (ee::Math::isPointInArea(pos, bound))
			return tris[i];
	}
	return NULL;
}

} // eanim