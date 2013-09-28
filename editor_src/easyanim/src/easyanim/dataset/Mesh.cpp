#include "Mesh.h"

namespace eanim
{

Mesh::Mesh(float width, float height)
{
	m_halfWidth = width * 0.5f;
	m_halfHeight = height * 0.5f;
	m_dt = NULL;
	m_isMerged = false;
}

Mesh::Mesh(const Mesh& mesh)
{
	m_tris.reserve(mesh.m_tris.size());
	for (size_t i = 0, n = mesh.m_tris.size(); i < n; ++i)
		m_tris.push_back(new MeshTri(*mesh.m_tris[i]));

	m_dt = NULL;

	m_halfWidth = mesh.m_halfWidth;
	m_halfHeight = mesh.m_halfHeight;

	m_isMerged = false;
}

Mesh::~Mesh()
{
	clear();
}

Mesh* Mesh::clone()
{
	return new Mesh(*this);
}

void Mesh::loadFromTextFile(std::ifstream& fin)
{
	clear();

	fin >> m_isMerged;
	
	size_t size;
	std::string flag;
	fin >> flag >> size;
	for (size_t i = 0; i < size; ++i)
	{
		d2d::Vector pos;
		fin >> pos;
		insertNode(pos);
	}
}

void Mesh::storeToTextFile(std::ofstream& fout) const
{
	fout << m_isMerged << '\n';

	const std::set<DT::Node*, DT::NodeCmp>& nodes = m_dt->getAllNodes();
	fout << "nodes " << nodes.size() << '\n';
	std::set<DT::Node*, DT::NodeCmp>::const_iterator itr = nodes.begin();
	for ( ; itr != nodes.end(); ++itr)
		fout << (*itr)->m_pos << " ";
	fout << '\n';
}

void Mesh::insertNode(const d2d::Vector& pos)
{
	if (!m_dt)
	{
		std::vector<d2d::Vector> bound;
		bound.push_back(d2d::Vector(-m_halfWidth, -m_halfHeight));
		bound.push_back(d2d::Vector( m_halfWidth, -m_halfHeight));
		bound.push_back(d2d::Vector( m_halfWidth,  m_halfHeight));
		bound.push_back(d2d::Vector(-m_halfWidth,  m_halfHeight));
		m_dt = new DT(bound, false);
	}

	if (fabs(pos.x) < m_halfWidth && fabs(pos.y) < m_halfHeight)
	{
		m_dt->insertNode(pos);
		loadTrisFromDT();
	}
}

void Mesh::deleteNode(const d2d::Vector& pos)
{
	std::vector<d2d::Vector> deletePos;
	deletePos.push_back(pos);
	m_dt->deleteNodes(deletePos);
	loadTrisFromDT();
}

const std::vector<MeshTri*>& Mesh::getAllTris() const
{
	return m_tris;
}

bool Mesh::isMerged() const
{
	return m_isMerged;
}

void Mesh::mergeNodes()
{
	if (m_isMerged) return;
	else m_isMerged = true;

	std::set<MeshNode*, MeshNodeCmp> nodesUnique;

	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTri* tri = m_tris[i];
		for (size_t j = 0; j < 3; ++j)
		{
			std::set<MeshNode*, MeshNodeCmp>::iterator itr;
			itr = nodesUnique.find(tri->m_nodes[j]);
			if (itr == nodesUnique.end())
			{
				MeshNode* node = new MeshNode(*tri->m_nodes[j]);
				nodesUnique.insert(node);
			}
		}
	}

	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTri* tri = m_tris[i];
		for (size_t j = 0; j < 3; ++j)
		{
			std::set<MeshNode*, MeshNodeCmp>::iterator itr;
			itr = nodesUnique.find(tri->m_nodes[j]);
			assert(itr != nodesUnique.end());
			tri->m_nodes[j]->release();
			tri->m_nodes[j] = *itr;
			tri->m_nodes[j]->retain();
		}
	}

	std::set<MeshNode*, MeshNodeCmp>::iterator itr = nodesUnique.begin();
	for ( ; itr != nodesUnique.end(); ++itr)
		(*itr)->release();
}

void Mesh::separateNodes()
{
	if (!m_isMerged) return;
	else m_isMerged = false;

	for (size_t i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTri* tri = m_tris[i];
		for (size_t j = 0; j < 3; ++j)
		{
			MeshNode* node = new MeshNode(*tri->m_nodes[j]);
			tri->m_nodes[j]->release();
			tri->m_nodes[j] = node;
		}
	}
}

void Mesh::tween(const Mesh& start, const Mesh& end, float process)
{
	const std::vector<MeshTri*>& sTris = start.getAllTris(),
		eTris = end.getAllTris();
	assert(sTris.size() == eTris.size() && sTris.size() == m_tris.size());
	for (size_t i = 0, n = sTris.size(); i < n; ++i)
	{
		std::vector<MeshNode*> sNodes, eNodes, tNodes;
		sTris[i]->getNodes(sNodes);
		eTris[i]->getNodes(eNodes);
		m_tris[i]->getNodes(tNodes);
		for (size_t j = 0; j < 3; ++j)
		{
			d2d::Vector offset = (eNodes[j]->projCoords - sNodes[j]->projCoords) * process;
			tNodes[j]->projCoords = sNodes[j]->projCoords + offset;
		}
	}
}

void Mesh::clear()
{
	for_each(m_tris.begin(), m_tris.end(), DeletePointerFunctor<MeshTri>());
	m_tris.clear();
	delete m_dt, m_dt = NULL;
}

void Mesh::loadTrisFromDT()
{
	if (!m_dt) return;

	m_isMerged = true;

	std::vector<d2d::Vector> bound;
	bound.push_back(d2d::Vector(-m_halfWidth, -m_halfHeight));
	bound.push_back(d2d::Vector( m_halfWidth, -m_halfHeight));
	bound.push_back(d2d::Vector( m_halfWidth,  m_halfHeight));
	bound.push_back(d2d::Vector(-m_halfWidth,  m_halfHeight));

	for_each(m_tris.begin(), m_tris.end(), DeletePointerFunctor<MeshTri>());
	m_tris.clear();

	//////////////////////////////////////////////////////////////////////////

	std::set<MeshNode*, MeshNodeCmp> nodesDst;
	const std::set<DT::Node*, DT::NodeCmp>& nodesSrc = m_dt->getAllNodes();
	std::set<DT::Node*, DT::NodeCmp>::const_iterator itrSrc = nodesSrc.begin();
	for ( ; itrSrc != nodesSrc.end(); ++itrSrc)
	{
		MeshNode* node = new MeshNode((*itrSrc)->m_pos, m_halfWidth, m_halfHeight);
		nodesDst.insert(node);
	}

	const std::vector<DT::Triangle*>& trisSrc = m_dt->getAllTris();
	for (size_t i = 0, n = trisSrc.size(); i < n; ++i)
	{
		DT::Triangle* src = trisSrc[i];
		std::vector<d2d::Vector> triBound;
		src->getNodesPos(triBound);

		d2d::Vector p = d2d::Math::getTriGravityCenter(triBound[0], triBound[1], triBound[2]);
		if (d2d::Math::isPointInArea(p, bound))
		{
			MeshTri* tri = new MeshTri;
			for (size_t j = 0; j < 3; ++j)
			{
				MeshNode tmp(triBound[j], m_halfWidth, m_halfHeight);
				std::set<MeshNode*, MeshNodeCmp>::iterator itr = nodesDst.find(&tmp);
				assert(itr != nodesDst.end());
				tri->m_nodes[j] = *itr;
				tri->m_nodes[j]->retain();
			}
			m_tris.push_back(tri);
		}
	}
	
	std::set<MeshNode*, MeshNodeCmp>::iterator itrDst = nodesDst.begin();
	for ( ; itrDst != nodesDst.end(); ++itrDst)
		(*itrDst)->release();
}

//////////////////////////////////////////////////////////////////////////
// class MeshTri
//////////////////////////////////////////////////////////////////////////

MeshTri::MeshTri()
{
	for (size_t i = 0; i < 3; ++i)
		m_nodes[i] = NULL;
}

MeshTri::MeshTri(const MeshTri& tri)
{
	for (size_t i = 0; i < 3; ++i)
		m_nodes[i] = new MeshNode(*tri.m_nodes[i]);
}

MeshTri::~MeshTri()
{
	for (size_t i = 0; i < 3; ++i)
		if (m_nodes[i]) 
			m_nodes[i]->release();
}

MeshTri* MeshTri::clone()
{
	return new MeshTri(*this);
}

void MeshTri::getNodes(std::vector<MeshNode*>& nodes)
{
	nodes.clear();
	for (size_t i = 0; i < 3; ++i)
		nodes.push_back(m_nodes[i]);
}

//////////////////////////////////////////////////////////////////////////
// class MeshNode
//////////////////////////////////////////////////////////////////////////

MeshNode::MeshNode()
{

}

MeshNode::MeshNode(const MeshNode& node)
{
	texCoords = node.texCoords;
	projCoords = node.projCoords;
}

MeshNode::MeshNode(const d2d::Vector& pos, float hWidth, float hHeight)
{
	initCoords(pos, hWidth, hHeight);
}

MeshNode& MeshNode::operator = (const MeshNode& node)
{
	texCoords = node.texCoords;
	projCoords = node.projCoords;
	return *this;
}

void MeshNode::loadFromTextFile(std::ifstream& fin)
{
	fin >> texCoords >> projCoords;
}

void MeshNode::storeToTextFile(std::ofstream& fout) const
{
	fout << texCoords << " " << projCoords;
}

void MeshNode::initCoords(const d2d::Vector& pos, float hWidth, float hHeight)
{
	projCoords = pos;
	texCoords.x = (pos.x + hWidth) / (hWidth * 2);
	texCoords.y = (pos.y + hHeight) / (hHeight * 2);
}

} // eanim