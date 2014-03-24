#include "MeshSymbol.h"

#include "render/PrimitiveDraw.h"
#include "common/MeshFileAdapter.h"
#include "common/FileNameTools.h"

namespace d2d
{

MeshSymbol::MeshSymbol()
	: m_image(NULL)
{
}

MeshSymbol::~MeshSymbol()
{
	m_image->release();
}

MeshSymbol* MeshSymbol::clone() const 
{ 
	return NULL; 
}

void MeshSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void MeshSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void MeshSymbol::reloadTexture() const
{
	m_image->reload();
}

void MeshSymbol::draw(const Colorf& mul, const Colorf& add,
					  const ISprite* sprite/* = NULL*/) const
{
//	PrimitiveDraw::drawTriangles(m_image->textureID(), m_vertices, m_texCoords);

	PrimitiveDraw::drawTrianglesSlow(m_image->textureID(), m_vertices, m_texCoords);
}

Rect MeshSymbol::getSize(const ISprite* sprite/* = NULL*/) const
{
	Rect rect;
	for (size_t i = 0, n = m_vertices.size(); i < n; ++i)
		rect.combine(m_vertices[i]);
	return rect;
}

const std::vector<Vector>& MeshSymbol::getVertices() const
{
	return m_vertices;
}

void MeshSymbol::loadResources()
{
	d2d::MeshFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	wxString filepath = FilenameTools::getExistFilepath(adapter.m_imagePath);

	m_image = ImageMgr::Instance()->getItem(filepath);

	m_vertices = adapter.m_vertices;
	m_texCoords = adapter.m_texCoords;
}

} // d2d