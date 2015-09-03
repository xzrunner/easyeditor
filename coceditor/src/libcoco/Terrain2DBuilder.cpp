#include "Terrain2DBuilder.h"
#include "PackAnimation.h"
#include "PackPicture.h"

namespace libcoco
{

Terrain2DBuilder::Terrain2DBuilder()
{
}

Terrain2DBuilder::~Terrain2DBuilder()
{
	std::map<const eterrain2d::Symbol*, const PackAnimation*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void Terrain2DBuilder::Traverse(d2d::IVisitor& visitor) const
{
	std::map<const eterrain2d::Symbol*, const PackAnimation*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackAnimation*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* Terrain2DBuilder::Create(const eterrain2d::Symbol* symbol)
{
	std::map<const eterrain2d::Symbol*, const PackAnimation*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackAnimation* node = new PackAnimation;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	return node;
}

void Terrain2DBuilder::Load(const eterrain2d::Symbol* symbol, PackAnimation* anim)
{
	const std::vector<eterrain2d::OceanMesh*> oceans = symbol->GetOceans();
	// todo
	if (oceans.size() != 1) {
		throw d2d::Exception("Terrain2DBuilder::Load oceans.size() != 1");
	}

	eterrain2d::OceanMesh* ocean = oceans[0];
	
	// todo
	ocean->OpenWave(false);
	ocean->OpenBlend(false);

	static const float FPS = 30;
	if (ocean->GetUVMoveSpeed().y == 0) {
		throw d2d::Exception("Terrain2DBuilder::Load ocean speed 0.");
	}
	int frame = (int)(fabs(FPS / ocean->GetUVMoveSpeed().y));

	ocean->Build();

	for (int i = 0; i < frame; ++i)
	{
		PackAnimation::Frame frame;
		PackPicture* pic = new PackPicture;

		const std::vector<eterrain2d::MeshShape*>& meshes = ocean->GetMeshes();
		for (int j = 0, m = meshes.size(); j < m; ++j)
		{
			const eterrain2d::MeshShape* mesh = meshes[j];
			const std::vector<emesh::Triangle*>& tris = mesh->GetTriangles();
			for (int k = 0, l = tris.size(); k < l; ++k)
			{
				emesh::Triangle* tri = tris[k];

				PackPicture::Quad quad;

				quad.img = ocean->GetImage0()->getImage();

				for (int i = 0; i < 3; ++i) {
					quad.texture_coord[i] = tri->nodes[i]->uv;
				}
				quad.texture_coord[3] = quad.texture_coord[2];

				for (int i = 0; i < 3; ++i) {
					quad.screen_coord[i] = tri->nodes[i]->xy;
				}
				quad.screen_coord[3] = quad.screen_coord[2];

				pic->quads.push_back(quad);
			}
		}

		int comp_idx = anim->components.size();
		PackAnimation::Component comp;
		comp.node = pic;
		anim->components.push_back(comp);

		PackAnimation::Part part;
		part.comp_idx = comp_idx;
		frame.parts.push_back(part);

		anim->frames.push_back(frame);

		ocean->Update(1 / FPS);
	}

	PackAnimation::Action action;
	action.size = anim->frames.size();
	anim->actions.push_back(action);
}

}