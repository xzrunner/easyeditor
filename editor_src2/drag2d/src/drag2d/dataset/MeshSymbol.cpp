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

}

MeshSymbol* MeshSymbol::clone() 
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

void MeshSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
//	PrimitiveDraw::drawTriangles(m_image->textureID(), m_vertices, m_texCoords);

	PrimitiveDraw::drawTrianglesSlow(m_image->textureID(), m_vertices, m_texCoords);
}

float MeshSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float MeshSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
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