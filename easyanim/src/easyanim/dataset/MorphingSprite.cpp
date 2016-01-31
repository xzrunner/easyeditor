#include "MorphingSprite.h"
#include "Symbol.h"
#include "Mesh.h"
#include "PartSkeleton.h"
#include "SpriteFactory.h"

#include "frame/Context.h"
#include "view/LibraryPanel.h"

namespace eanim
{

MorphingSprite::MorphingSprite()
{
	m_symbol = NULL;
	m_mesh = NULL;
	m_skeleton = NULL;
}

MorphingSprite::MorphingSprite(Symbol* symbol, const ee::Vector& pos, bool isCopyMesh)
{
	m_pos = pos;
	m_symbol = symbol;
	initBounding(symbol);

	m_mesh = NULL;
	m_skeleton = NULL;

	if (isCopyMesh)
	{
		Mesh* mesh = symbol->getMesh();
		if (mesh) m_mesh = mesh->clone();

		PartSkeleton* skeleton = symbol->getSkeleton();
		if (skeleton) m_skeleton = skeleton->clone();
	}
}

MorphingSprite::MorphingSprite(const MorphingSprite& sprite)
	: Sprite(sprite)
{
	m_pos = sprite.m_pos;
	m_angle = sprite.m_angle;
	m_symbol = sprite.m_symbol;

	if (sprite.m_mesh) m_mesh = sprite.m_mesh->clone();
	else m_mesh = NULL;

	if (sprite.m_skeleton) m_skeleton = sprite.m_skeleton->clone();
	else m_skeleton = NULL;
}

MorphingSprite::~MorphingSprite()
{
	clear();
}

MorphingSprite* MorphingSprite::clone() const
{
	return new MorphingSprite(*this);
}

void MorphingSprite::loadFromTextFile(std::ifstream& fin)
{
	clear();

	fin >> m_pos >> m_angle;

	int index;
	fin >> index;
	assert(index != -1);
	m_symbol = Context::Instance()->library->getSymbol(index);

	initBounding(m_symbol);

	Mesh* mesh = m_symbol->getMesh();
	if (mesh) 
	{
		m_mesh = mesh->clone();
		const std::vector<MeshTri*>& tris = m_mesh->getAllTris();
		std::string flag;
		size_t size;
		fin >> flag >> size;
		assert(tris.size() == size);
		for (size_t i = 0, n = tris.size(); i < n; ++i)
			for (size_t j = 0; j < 3; ++j)
				fin >> tris[i]->m_nodes[j]->projCoords;
	}

	PartSkeleton* skeleton = m_symbol->getSkeleton();
	if (skeleton) 
	{
		m_skeleton = new PartSkeleton(m_symbol->getWidth(), m_symbol->getHeight());
		m_skeleton->loadFromTextFile(fin);
	}
}

void MorphingSprite::storeToTextFile(std::ofstream& fout) const
{
// 	fout << SpriteFactory::e_morphing << " "
// 		<< m_pos << " " << m_angle << " "
// 		<< library.getIndex(m_symbol) << '\n';
	if (m_mesh)
	{
		const std::vector<MeshTri*>& tris = m_mesh->getAllTris();
		fout << "tris " << tris.size() << '\n';
		for (size_t i = 0, n = tris.size(); i < n; ++i)
			for (size_t j = 0; j < 3; ++j)
				fout << tris[i]->m_nodes[j]->projCoords << " ";
		fout << '\n';
	}
	if (m_skeleton)
	{
		m_skeleton->storeToTextFile(fout);
	}
}

void MorphingSprite::reloadTexture()
{
	m_symbol->reloadTexture();
}

void MorphingSprite::clear()
{
	delete m_mesh;
	delete m_skeleton;
}

void MorphingSprite::initBounding(Symbol* symbol)
{
	const float hWidth = symbol->getWidth() * 0.5f,
		hHeight = symbol->getHeight() * 0.5f;

	m_bounding = ee::BVFactory::createBV(ee::e_obb);
	ee::Rect aabb;
	aabb.xMin = -hWidth;
	aabb.xMax = hWidth;
	aabb.yMin = -hHeight;
	aabb.yMax = hHeight;
	aabb.translate(m_pos);
	m_bounding->initFromRect(aabb);
}

} // eanim