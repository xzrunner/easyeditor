#include "PackNodeFactory.h"
#include "PackUI.h"

#include <ee/ImageSprite.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/FetchAllVisitor.h>
#include <ee/std_functor.h>

// picture
#include "ImageBuilder.h"
#include "Scale9Builder.h"
#include <easyscale9.h>
#include "IconBuilder.h"
#include <easyicon.h>
#include "TextureBuilder.h"
#include <easytexture.h>

// label
#include "TextBuilder.h"
#include <easytext.h>

// animation
#include "ComplexBuilder.h"
#include <easycomplex.h>
#include "AnimBuilder.h"
#include <easyanim.h>
#include "Terrain2DBuilder.h"
#include <easyterrain2d.h>

// anchor
#include "AnchorBuilder.h"

// particle3d
#include "Particle3DBuilder.h"
#include <easyparticle3d.h>

// particle2d
#include "Particle2DBuilder.h"
#include <easyparticle2d.h>

// p3d spr
#include "P3dSprBuilder.h"

// clipbox
#include "ClipboxBuilder.h"

// shape
#include "ShapeBuilder.h"

// mesh
#include "MeshBuilder.h"

// mesh spr
#include "MeshSprBuilder.h"

// mask
#include "MaskBuilder.h"
#include <easymask.h>

// trail
#include "TrailBuilder.h"
#include <easytrail.h>

#include <spritepack.h>

namespace erespacker
{

PackNodeFactory* PackNodeFactory::m_instance = NULL;

PackNodeFactory::PackNodeFactory()
{
	// clipbox
	m_builders.push_back(m_clipbox_builder = new ClipboxBuilder);

	// picture
	m_builders.push_back(m_img_builder = new ImageBuilder);
	m_builders.push_back(m_scale9_builder = new Scale9Builder);
	m_builders.push_back(m_icon_builder = new IconBuilder);
	m_builders.push_back(m_tex_builder = new TextureBuilder);

	// label
	m_builders.push_back(m_text_builder = new TextBuilder);

	// animation
	m_builders.push_back(m_complex_builder = new ComplexBuilder(m_export_set, m_clipbox_builder));
	m_builders.push_back(m_anim_builder = new AnimBuilder(m_export_set));
	m_builders.push_back(m_terrain2d_builder = new Terrain2DBuilder);

	// anchor
	m_builders.push_back(m_anchor_builder = new AnchorBuilder);

	// particle3d
	m_builders.push_back(m_particle3d_builder = new Particle3DBuilder);

	// particle2d
	m_builders.push_back(m_particle2d_builder = new Particle2DBuilder(m_export_set));

	// p3d spr
	m_builders.push_back(m_p3d_spr_builder = new P3dSprBuilder(m_export_set));

	// shape
	m_builders.push_back(m_shape_builder = new ShapeBuilder);

	// mesh
	m_builders.push_back(m_mesh_builder = new MeshBuilder);

	// mesh spr
	m_builders.push_back(m_mesh_spr_builder = new MeshSprBuilder);

	// mask
	m_builders.push_back(m_mask_builder = new MaskBuilder);

	// trail
	m_builders.push_back(m_trail_builder = new TrailBuilder(m_export_set));
}

const IPackNode* PackNodeFactory::Create(const ee::Sprite* spr)
{
	const IPackNode* node = NULL;

	// anchor
	if (spr->IsAnchor()) {
		node = m_anchor_builder->Create(spr);
	}
	
	// picture
	else if (const ee::ImageSprite* image = dynamic_cast<const ee::ImageSprite*>(spr)) {
		node = m_img_builder->Create(image);
	} else if (const escale9::Sprite* scale9 = dynamic_cast<const escale9::Sprite*>(spr)) {
		node = m_scale9_builder->Create(scale9);
	} else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) {
		node = m_icon_builder->Create(icon);
	} else if (const etexture::Sprite* tex = dynamic_cast<const etexture::Sprite*>(spr)) {
		const etexture::Symbol* sym = &tex->GetSymbol();
		if (m_tex_builder->CanHandle(sym)) {
			node = m_tex_builder->Create(sym);
		} else if (m_shape_builder->CanHandle(sym)) {
			node = m_shape_builder->Create(sym);
		} else {
			throw ee::Exception("PackNodeFactory::Create unknown etexture.");
		}
	}

	// label
	else if (const etext::Sprite* text = dynamic_cast<const etext::Sprite*>(spr)) {
		node = m_text_builder->Create(text);
	}

	// animation
	else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		node = m_complex_builder->Create(&complex->GetSymbol());
	} else if (const eanim::Sprite* anim = dynamic_cast<const eanim::Sprite*>(spr)) {
		node = m_anim_builder->Create(&anim->GetSymbol());
	} else if (const eterrain2d::Sprite* terr2d = dynamic_cast<const eterrain2d::Sprite*>(spr)) {
		node = m_terrain2d_builder->Create(&terr2d->GetSymbol());
	}

	// p3d spr
	else if (const eparticle3d::Sprite* p3d = dynamic_cast<const eparticle3d::Sprite*>(spr)) {
		node = m_p3d_spr_builder->Create(p3d);
	}

	// particle2d
	else if (const eparticle2d::Sprite* p2d = dynamic_cast<const eparticle2d::Sprite*>(spr)) {
		node = m_particle2d_builder->Create(&p2d->GetSymbol());
	}

	// mesh spr
	else if (const emesh::Sprite* mesh = dynamic_cast<const emesh::Sprite*>(spr)) {
		node = m_mesh_spr_builder->Create(mesh);
	}

	// mask
	else if (const emask::Sprite* mask = dynamic_cast<const emask::Sprite*>(spr)) {
		node = m_mask_builder->Create(&mask->GetSymbol());
	}

	else {
		throw ee::Exception("PackNodeFactory::Create unknown sprite type.");
	}
	
	node->SetFilepath(ee::FileHelper::GetRelativePath(m_files_dir, spr->GetSymbol().GetFilepath()));

	if (node->GetSprID() > ANCHOR_ID) {
		throw ee::Exception("PackNodeFactory::Create node id over ANCHOR_ID.");
	}

	return node;
}

const IPackNode* PackNodeFactory::Create(const ee::Symbol* symbol)
{
	const IPackNode* node = NULL;

	// picture
	if (const ee::ImageSymbol* img_symbol = dynamic_cast<const ee::ImageSymbol*>(symbol)) {
		ee::ImageSprite* img_spr = new ee::ImageSprite(const_cast<ee::ImageSymbol*>(img_symbol));
		node = m_img_builder->Create(img_spr);
		img_spr->Release();
	} else if (const etexture::Symbol* tex = dynamic_cast<const etexture::Symbol*>(symbol)) {
		node = m_tex_builder->Create(tex);
	}

	// animation
	else if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol)) {
		node = m_complex_builder->Create(complex);
	} else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(symbol)) {
		node = m_anim_builder->Create(anim);
	} else if (const eterrain2d::Symbol* terr2d = dynamic_cast<const eterrain2d::Symbol*>(symbol)) {
		node = m_terrain2d_builder->Create(terr2d);
	}

	// particle3d
	else if (const eparticle3d::Symbol* p3d = dynamic_cast<const eparticle3d::Symbol*>(symbol)) {
		node = m_particle3d_builder->Create(p3d, m_p3d_spr_builder);
	}

	// particle2d
	else if (const eparticle2d::Symbol* p2d = dynamic_cast<const eparticle2d::Symbol*>(symbol)) {
		node = m_particle2d_builder->Create(p2d);
	}

	// mesh
	else if (const emesh::Symbol* mesh = dynamic_cast<const emesh::Symbol*>(symbol)) {
		node = m_mesh_builder->Create(mesh);
	}

	// trail
	else if (const etrail::Symbol* trail = dynamic_cast<const etrail::Symbol*>(symbol)) {
		node = m_trail_builder->Create(trail);
	}

	else {
		throw ee::Exception("PackNodeFactory::Create unknown symbol type.");
	}

	node->SetFilepath(ee::FileHelper::GetRelativePath(m_files_dir, symbol->GetFilepath()));

	if (node->GetSprID() > ANCHOR_ID) {
		throw ee::Exception("PackNodeFactory::Create node id over ANCHOR_ID.");
	}

	PackUI::Instance()->OnKnownPackID(symbol->GetFilepath(), node->GetSprID());

	return node;
}

void PackNodeFactory::GetAllNodes(std::vector<IPackNode*>& nodes) const
{
	nodes.clear();
	for (int i = 0, n = m_builders.size(); i < n; ++i) {
		m_builders[i]->Traverse(ee::FetchAllVisitor<IPackNode>(nodes));
	}
}

void PackNodeFactory::Release()
{
	m_files_dir = "";
	m_export_set.Clear();
	for_each(m_builders.begin(), m_builders.end(), ee::DeletePointerFunctor<INodeBuilder>());
	m_instance = NULL;
}

PackNodeFactory* PackNodeFactory::Instance()
{
	if (!m_instance) {
		m_instance = new PackNodeFactory;
	}
	return m_instance;
}

}