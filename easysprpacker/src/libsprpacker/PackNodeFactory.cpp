#include "PackNodeFactory.h"
#include "SymBuilder.h"
#include "SprBuilder.h"
#include "LabelBuilder.h"

#include "PackImage.h"
#include "PackScale9.h"
#include "PackScale9Spr.h"
#include "PackIcon.h"
#include "PackIconSpr.h"
#include "PackTexture.h"
#include "PackTextureSpr.h"
#include "PackComplex.h"
#include "PackComplexSpr.h"
#include "PackAnimation.h"
#include "PackAnimationSpr.h"
#include "PackParticle3d.h"
#include "PackParticle3dSpr.h"
#include "PackParticle2d.h"
#include "PackParticle2dSpr.h"
#include "PackShape.h"
#include "PackShapeSpr.h"
#include "PackMesh.h"
#include "PackMeshSpr.h"
#include "PackMask.h"
#include "PackMaskSpr.h"
#include "PackTrail.h"
#include "PackTrailSpr.h"

#include <easyscale9.h>
#include <easyicon.h>
#include <easytexture.h>
#include <easytext.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easyshape.h>
#include <easymesh.h>
#include <easymask.h>
#include <easytrail.h>

#include <ee/ImageSprite.h>
#include <ee/ImageSymbol.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/FetchAllVisitor.h>

#include <simp/simp_types.h>

namespace esprpacker
{

SINGLETON_DEFINITION(PackNodeFactory);

SymBuilder<ee::ImageSymbol, PackImage>*										IMAGE_BUILDER;
SymBuilder<escale9::Symbol, PackScale9>*									SCALE9_BUILDER;
SymBuilder<eicon::Symbol, PackIcon>*										ICON_BUILDER;
SymBuilder<etexture::Symbol, PackTexture>*									TEXTURE_BUILDER;
SymBuilder<ecomplex::Symbol, PackComplex>*									COMPLEX_BUILDER;
SymBuilder<eanim::Symbol, PackAnimation>*									ANIM_BUILDER;
SymBuilder<eparticle3d::Symbol, PackParticle3d>*							P3D_BUILDER;
SymBuilder<eparticle2d::Symbol, PackParticle2d>*							P2D_BUILDER;
SymBuilder<eshape::Symbol, PackShape>*										SHAPE_BUILDER;
SymBuilder<emesh::Symbol, PackMesh>*										MESH_BUILDER;
SymBuilder<emask::Symbol, PackMask>*										MASK_BUILDER;
SymBuilder<etrail::Symbol, PackTrail>*										TRAIL_BUILDER;

SprBuilder<escale9::Symbol, escale9::Sprite, PackScale9Spr>*				SCALE9_SPR_BUILDER;
SprBuilder<eicon::Symbol, eicon::Sprite, PackIconSpr>*						ICON_SPR_BUILDER;
SprBuilder<etexture::Symbol, etexture::Sprite, PackTextureSpr>*				TEXTURE_SPR_BUILDER;
SprBuilder<ecomplex::Symbol, ecomplex::Sprite, PackComplexSpr>*				COMPLEX_SPR_BUILDER;
SprBuilder<eanim::Symbol, eanim::Sprite, PackAnimationSpr>*					ANIM_SPR_BUILDER;
SprBuilder<eparticle3d::Symbol, eparticle3d::Sprite, PackParticle3dSpr>*	P3D_SPR_BUILDER;
SprBuilder<eparticle2d::Symbol, eparticle2d::Sprite, PackParticle2dSpr>*	P2D_SPR_BUILDER;
SprBuilder<eshape::Symbol, eshape::Sprite, PackShapeSpr>*					SHAPE_SPR_BUILDER;
SprBuilder<emesh::Symbol, emesh::Sprite, PackMeshSpr>*						MESH_SPR_BUILDER;
SprBuilder<emask::Symbol, emask::Sprite, PackMaskSpr>*						MASK_SPR_BUILDER;
SprBuilder<etrail::Symbol, etrail::Sprite, PackTrailSpr>*					TRAIL_SPR_BUILDER;

PackNodeFactory::PackNodeFactory()
{
	IMAGE_BUILDER		= new SymBuilder<ee::ImageSymbol, PackImage>();
	SCALE9_BUILDER		= new SymBuilder<escale9::Symbol, PackScale9>();
	ICON_BUILDER		= new SymBuilder<eicon::Symbol, PackIcon>();
	TEXTURE_BUILDER		= new SymBuilder<etexture::Symbol, PackTexture>();
	COMPLEX_BUILDER		= new SymBuilder<ecomplex::Symbol, PackComplex>(true);
	ANIM_BUILDER		= new SymBuilder<eanim::Symbol, PackAnimation>(true);
	P3D_BUILDER			= new SymBuilder<eparticle3d::Symbol, PackParticle3d>(true);
	P2D_BUILDER			= new SymBuilder<eparticle2d::Symbol, PackParticle2d>(true);
	SHAPE_BUILDER		= new SymBuilder<eshape::Symbol, PackShape>();
	MESH_BUILDER		= new SymBuilder<emesh::Symbol, PackMesh>();
	MASK_BUILDER		= new SymBuilder<emask::Symbol, PackMask>();
	TRAIL_BUILDER		= new SymBuilder<etrail::Symbol, PackTrail>(true);

	SCALE9_SPR_BUILDER	= new SprBuilder<escale9::Symbol, escale9::Sprite, PackScale9Spr>();
	ICON_SPR_BUILDER	= new SprBuilder<eicon::Symbol, eicon::Sprite, PackIconSpr>();
	TEXTURE_SPR_BUILDER	= new SprBuilder<etexture::Symbol, etexture::Sprite, PackTextureSpr>();
	COMPLEX_SPR_BUILDER	= new SprBuilder<ecomplex::Symbol, ecomplex::Sprite, PackComplexSpr>();
	ANIM_SPR_BUILDER	= new SprBuilder<eanim::Symbol, eanim::Sprite, PackAnimationSpr>();
	P3D_SPR_BUILDER		= new SprBuilder<eparticle3d::Symbol, eparticle3d::Sprite, PackParticle3dSpr>();
	P2D_SPR_BUILDER		= new SprBuilder<eparticle2d::Symbol, eparticle2d::Sprite, PackParticle2dSpr>();
	SHAPE_SPR_BUILDER	= new SprBuilder<eshape::Symbol, eshape::Sprite, PackShapeSpr>();
	MESH_SPR_BUILDER	= new SprBuilder<emesh::Symbol, emesh::Sprite, PackMeshSpr>();
	MASK_SPR_BUILDER	= new SprBuilder<emask::Symbol, emask::Sprite, PackMaskSpr>();
	TRAIL_SPR_BUILDER	= new SprBuilder<etrail::Symbol, etrail::Sprite, PackTrailSpr>();
}

PackNodeFactory::~PackNodeFactory()
{
}

const PackNode* PackNodeFactory::Create(const ee::Sprite* spr)
{
	const PackNode* node = NULL;

	if (spr->IsAnchor()) {
		// todo
	}
	// image
	else if (const ee::ImageSprite* image = dynamic_cast<const ee::ImageSprite*>(spr)) {
		node = IMAGE_BUILDER->Create(dynamic_cast<const ee::ImageSymbol*>(image->GetSymbol()));
	}
	// scale9 spr
	else if (const escale9::Sprite* s9 = dynamic_cast<const escale9::Sprite*>(spr)) {
		node = SCALE9_SPR_BUILDER->Create(s9);
	}
	// icon spr
	else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) {
		node = ICON_SPR_BUILDER->Create(icon);
	}
	// texture
	else if (const etexture::Sprite* texture = dynamic_cast<const etexture::Sprite*>(spr)) {
		node = TEXTURE_SPR_BUILDER->Create(texture);
	}
	// label
	else if (const etext::Sprite* label = dynamic_cast<const etext::Sprite*>(spr)) {
		node = LabelBuilder::Instance()->Create(label);
	}
	// complex
	else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		node = COMPLEX_SPR_BUILDER->Create(complex);
	}
	// animation
	else if (const eanim::Sprite* anim = dynamic_cast<const eanim::Sprite*>(spr)) {
		node = ANIM_SPR_BUILDER->Create(anim);
	}
	// particle3d
	else if (const eparticle3d::Sprite* p3d = dynamic_cast<const eparticle3d::Sprite*>(spr)) {
		node = P3D_SPR_BUILDER->Create(p3d);
	}
	// particle2d
	else if (const eparticle2d::Sprite* p2d = dynamic_cast<const eparticle2d::Sprite*>(spr)) {
		node = P2D_SPR_BUILDER->Create(p2d);
	}
	// shape
	else if (const eshape::Sprite* shape = dynamic_cast<const eshape::Sprite*>(spr)) {
		node = SHAPE_SPR_BUILDER->Create(shape);
	}
	// mesh
	else if (const emesh::Sprite* mesh = dynamic_cast<const emesh::Sprite*>(spr)) {
		node = MESH_SPR_BUILDER->Create(mesh);
	}
	// mask
	else if (const emask::Sprite* mask = dynamic_cast<const emask::Sprite*>(spr)) {
		node = MASK_SPR_BUILDER->Create(mask);
	}
	// trail
	else if (const etrail::Sprite* trail = dynamic_cast<const etrail::Sprite*>(spr)) {
		node = TRAIL_SPR_BUILDER->Create(trail);
	}

	else {
		throw ee::Exception("PackNodeFactory::Create unknown sprite type.");
	}

	node->SetFilepath(ee::FileHelper::GetRelativePath(m_files_dir, dynamic_cast<const  ee::Symbol*>(spr->GetSymbol())->GetFilepath()));

	return node;
}

const PackNode* PackNodeFactory::Create(const ee::Symbol* sym)
{
	const PackNode* node = NULL;

	// image
	if (const ee::ImageSymbol* image = dynamic_cast<const ee::ImageSymbol*>(sym)) {
		node = IMAGE_BUILDER->Create(image);
	}
	// scale9
	else if (const escale9::Symbol* s9 = dynamic_cast<const escale9::Symbol*>(sym)) {
		node = SCALE9_BUILDER->Create(s9);
	}
	// icon
	else if (const eicon::Symbol* icon = dynamic_cast<const eicon::Symbol*>(sym)) {
		node = ICON_BUILDER->Create(icon);
	}
	// texture
	else if (const etexture::Symbol* texture = dynamic_cast<const etexture::Symbol*>(sym)) {
		node = TEXTURE_BUILDER->Create(texture);
	}
	// complex
	else if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(sym)) {
		node = COMPLEX_BUILDER->Create(complex);
	}
	// anim
	else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(sym)) {
		node = ANIM_BUILDER->Create(anim);
	}
	// particle3d
	else if (const eparticle3d::Symbol* p3d = dynamic_cast<const eparticle3d::Symbol*>(sym)) {
		node = P3D_BUILDER->Create(p3d);
	}
	// particle2d
	else if (const eparticle2d::Symbol* p2d = dynamic_cast<const eparticle2d::Symbol*>(sym)) {
		node = P2D_BUILDER->Create(p2d);
	}
	// shape
	else if (const eshape::Symbol* shape = dynamic_cast<const eshape::Symbol*>(sym)) {
		node = SHAPE_BUILDER->Create(shape);
	}
	// mesh
	else if (const emesh::Symbol* mesh = dynamic_cast<const emesh::Symbol*>(sym)) {
		node = MESH_BUILDER->Create(mesh);
	}
	// mask
	else if (const emask::Symbol* mask = dynamic_cast<const emask::Symbol*>(sym)) {
		node = MASK_BUILDER->Create(mask);
	}
	// trail
	else if (const etrail::Symbol* trail = dynamic_cast<const etrail::Symbol*>(sym)) {
		node = TRAIL_BUILDER->Create(trail);
	}

	else {
		throw ee::Exception("PackNodeFactory::Create unknown symbol type.");
	}

	node->SetFilepath(ee::FileHelper::GetRelativePath(m_files_dir, sym->GetFilepath()));

	return node;
}

void PackNodeFactory::FetchAll(std::vector<PackNode*>& nodes) const
{
	nodes.clear();

	std::vector<NodeBuilder*> builders;
	FetchAllBuilder(builders);
	for (int i = 0, n = builders.size(); i < n; ++i) {
		builders[i]->Traverse(ee::FetchAllVisitor<PackNode>(nodes));
	}
}

void PackNodeFactory::FetchAllBuilder(std::vector<NodeBuilder*>& builders)
{
	builders.clear();

	builders.push_back(IMAGE_BUILDER);
	builders.push_back(SCALE9_BUILDER);
	builders.push_back(ICON_BUILDER);
	builders.push_back(TEXTURE_BUILDER);
	builders.push_back(LabelBuilder::Instance());
	builders.push_back(COMPLEX_BUILDER);
	builders.push_back(ANIM_BUILDER);
	builders.push_back(P3D_BUILDER);
	builders.push_back(P2D_BUILDER);
	builders.push_back(SHAPE_BUILDER);
	builders.push_back(MESH_BUILDER);
	builders.push_back(MASK_BUILDER);
	builders.push_back(TRAIL_BUILDER);

	builders.push_back(SCALE9_SPR_BUILDER);
	builders.push_back(ICON_SPR_BUILDER);
	builders.push_back(TEXTURE_SPR_BUILDER);
	builders.push_back(COMPLEX_SPR_BUILDER);
	builders.push_back(ANIM_SPR_BUILDER);
	builders.push_back(P3D_SPR_BUILDER);
	builders.push_back(P2D_SPR_BUILDER);
	builders.push_back(SHAPE_SPR_BUILDER);
	builders.push_back(MESH_SPR_BUILDER);
	builders.push_back(MASK_SPR_BUILDER);
	builders.push_back(TRAIL_SPR_BUILDER);
}

}