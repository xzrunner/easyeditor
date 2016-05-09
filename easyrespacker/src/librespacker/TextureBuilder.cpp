#include "TextureBuilder.h"
#include "PackPicture.h"

#include <easyshape.h>
#include <easytexture.h>

#include <ee/Exception.h>
#include <ee/ImageSymbol.h>

namespace erespacker
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

void TextureBuilder::Traverse(ee::Visitor& visitor) const
{
	std::map<const etexture::Symbol*, const PackPicture*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackPicture*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

bool TextureBuilder::CanHandle(const etexture::Symbol* symbol) const
{
	const std::vector<ee::Shape*>& shapes = symbol->GetAllShapes();
	if (shapes.size() != 1) {
		return false;
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shapes[0]);
	if (!poly) {
		return false;
	}
	const eshape::TextureMaterial* material = dynamic_cast<const eshape::TextureMaterial*>(poly->GetMaterial());
	if (!material) {
		return false;
	}
	return true;
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
	const std::vector<ee::Shape*>& shapes = symbol->GetAllShapes();
	if (shapes.size() != 1) {
		throw ee::Exception("TextureBuilder::Load shapes.size(): %d filepath: %s", shapes.size(), symbol->GetFilepath().c_str());
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shapes[0]);
	if (!poly) {
		throw ee::Exception("TextureBuilder::Load !poly, filepath: %s", symbol->GetFilepath().c_str());
	}
	const eshape::TextureMaterial* material = dynamic_cast<const eshape::TextureMaterial*>(poly->GetMaterial());
	if (!material) {
		throw ee::Exception("TextureBuilder::Load !material, filepath: %s", symbol->GetFilepath().c_str());
	}

	const std::vector<sm::vec2>& vertices = material->GetTriangles();
	const std::vector<sm::vec2>& texcoords = material->GetTexcoords();
	if ((vertices.size() != texcoords.size()) || (vertices.size() % 3 != 0)) {
		throw ee::Exception("TextureBuilder::Load err meaterial, filepath: %s", symbol->GetFilepath().c_str());
	}
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		PackPicture::Quad quad;

		quad.img = material->GetImage()->GetImage();

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