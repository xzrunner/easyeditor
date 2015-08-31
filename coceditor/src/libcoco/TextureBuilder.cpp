#include "TextureBuilder.h"
#include "PackPicture.h"

#include <easyshape.h>

namespace libcoco
{

TextureBuilder::TextureBuilder()
{
}

TextureBuilder::~TextureBuilder()
{
	std::map<const etexture::Symbol*, const PackPicture*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void TextureBuilder::ToString(ebuilder::CodeGenerator& gen,
							  const TexturePacker& tp) const
{
	std::map<const etexture::Symbol*, const PackPicture*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		itr->second->ToString(gen, tp);
	}
}

const IPackNode* TextureBuilder::Create(const etexture::Symbol* symbol)
{
	std::map<const etexture::Symbol*, const PackPicture*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackPicture* node = new PackPicture;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	return node;
}

void TextureBuilder::Load(const etexture::Symbol* symbol, PackPicture* pic)
{
	const std::vector<d2d::IShape*>& shapes = symbol->GetAllShapes();
	if (shapes.size() != 1) {
		throw d2d::Exception("TextureBuilder::Load shapes.size(): %d", shapes.size());
	}
	libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shapes[0]);
	if (!poly) {
		throw d2d::Exception("TextureBuilder::Load !poly");
	}
	const libshape::TextureMaterial* material = dynamic_cast<const libshape::TextureMaterial*>(poly->GetMaterial());
	if (!material) {
		throw d2d::Exception("TextureBuilder::Load !material");
	}

	const std::vector<d2d::Vector>& vertices = material->GetTriangles();
	const std::vector<d2d::Vector>& texcoords = material->GetTexcoords();
	if ((vertices.size() != texcoords.size()) || (vertices.size() % 3 != 0)) {
		throw d2d::Exception("TextureBuilder::Load err meaterial");
	}
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		PackPicture::Quad quad;

		for (int j = 0; j < 3; ++j) {
			quad.texture_coord[j] = texcoords[i+j];
		}
		quad.texture_coord[3] = quad.texture_coord[2];

		for (int j = 0; j < 3; ++j) {
			quad.screen_coord[j] = vertices[i+j];
		}
		quad.screen_coord[3] = quad.screen_coord[2];		

		pic->quads.push_back(quad);
	}
}

}