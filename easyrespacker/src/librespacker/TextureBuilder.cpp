#include "TextureBuilder.h"
#include "PackPicture.h"
#include "PackUI.h"
#include "PackTag.h"

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
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void TextureBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackPicture*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

bool TextureBuilder::CanHandle(const std::shared_ptr<etexture::Symbol>& sym) const
{
	auto& polys = sym->GetPolygons();
	if (polys.size() != 1) {
		return false;
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(polys[0].get());
	if (!poly) {
		return false;
	}
	const eshape::TextureMaterial* material = dynamic_cast<const eshape::TextureMaterial*>(poly->GetMaterial());
	if (!material) {
		return false;
	}
	return true;
}

const IPackNode* TextureBuilder::Create(const std::shared_ptr<const etexture::Symbol>& sym)
{
	auto itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackPicture* node = new PackPicture;
	Load(sym, node);
	m_map_data.insert(std::make_pair(sym, node));

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

void TextureBuilder::Load(const std::shared_ptr<const etexture::Symbol>& sym, PackPicture* pic)
{
	auto& polys = sym->GetPolygons();
	if (polys.size() != 1) {
		throw ee::Exception("TextureBuilder::Load shapes.size(): %d filepath: %s", polys.size(), sym->GetFilepath().c_str());
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(polys[0].get());
	if (!poly) {
		throw ee::Exception("TextureBuilder::Load !poly, filepath: %s", sym->GetFilepath().c_str());
	}
	const eshape::TextureMaterial* material = dynamic_cast<const eshape::TextureMaterial*>(poly->GetMaterial());
	if (!material) {
		throw ee::Exception("TextureBuilder::Load !material, filepath: %s", sym->GetFilepath().c_str());
	}

	auto& vertices = material->GetTriangles();
	auto& texcoords = material->GetTexcoords();
	if ((vertices.size() != texcoords.size()) || (vertices.size() % 3 != 0)) {
		throw ee::Exception("TextureBuilder::Load err meaterial, filepath: %s", sym->GetFilepath().c_str());
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