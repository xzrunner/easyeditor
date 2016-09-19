#include "PackNodeFactory.h"

#include "ImageBuilder.h"
#include "Scale9Builder.h"
#include "Scale9SprBuilder.h"
#include "IconBuilder.h"
#include "IconSprBuilder.h"
#include "TextureBuilder.h"
#include "LabelBuilder.h"
#include "ComplexBuilder.h"
#include "ComplexSprBuilder.h"
#include "AnimationBuilder.h"
#include "AnimationSprBuilder.h"
#include "Particle3dBuilder.h"
#include "Particle3dSprBuilder.h"
#include "Particle2dBuilder.h"
#include "Particle2dSprBuilder.h"
#include "TrailBuilder.h"
#include "TrailSprBuilder.h"

#include <easyscale9.h>
#include <easyicon.h>
#include <easytexture.h>
#include <easytext.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyparticle3d.h>
#include <easyparticle2d.h>
#include <easytrail.h>

#include <ee/Sprite.h>
#include <ee/ImageSprite.h>
#include <ee/ImageSymbol.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/FetchAllVisitor.h>

namespace esprpacker
{

SINGLETON_DEFINITION(PackNodeFactory);

PackNodeFactory::PackNodeFactory()
{
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
		node = ImageBuilder::Instance()->Create(dynamic_cast<const ee::ImageSymbol*>(image->GetSymbol()));
	}
	// scale9 spr
	else if (const escale9::Sprite* s9 = dynamic_cast<const escale9::Sprite*>(spr)) {
		node = Scale9SprBuilder::Instance()->Create(s9);
	}
	// icon spr
	else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) {
		node = IconSprBuilder::Instance()->Create(icon);
	}
	// texture
	else if (const etexture::Sprite* texture = dynamic_cast<const etexture::Sprite*>(spr)) {
		node = TextureBuilder::Instance()->Create(dynamic_cast<const etexture::Symbol*>(texture->GetSymbol()));
	}
	// label
	else if (const etext::Sprite* label = dynamic_cast<const etext::Sprite*>(spr)) {
		node = LabelBuilder::Instance()->Create(label);
	}
	// complex
	else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		node = ComplexSprBuilder::Instance()->Create(complex);
	}
	// animation
	else if (const eanim::Sprite* anim = dynamic_cast<const eanim::Sprite*>(spr)) {
		node = AnimationSprBuilder::Instance()->Create(anim);
	}
	// particle3d
	else if (const eparticle3d::Sprite* p3d = dynamic_cast<const eparticle3d::Sprite*>(spr)) {
		node = Particle3dSprBuilder::Instance()->Create(p3d);
	}
	// particle2d
	else if (const eparticle2d::Sprite* p2d = dynamic_cast<const eparticle2d::Sprite*>(spr)) {
		node = Particle2dSprBuilder::Instance()->Create(p2d);
	}
	// trail
	else if (const etrail::Sprite* trail = dynamic_cast<const etrail::Sprite*>(spr)) {
		node = TrailSprBuilder::Instance()->Create(trail);
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
		node = ImageBuilder::Instance()->Create(image);
	}
	// scale9
	else if (const escale9::Symbol* s9 = dynamic_cast<const escale9::Symbol*>(sym)) {
		node = Scale9Builder::Instance()->Create(s9);
	}
	// icon
	else if (const eicon::Symbol* icon = dynamic_cast<const eicon::Symbol*>(sym)) {
		node = IconBuilder::Instance()->Create(icon);
	}
	// complex
	else if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(sym)) {
		node = ComplexBuilder::Instance()->Create(complex);
	}
	// anim
	else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(sym)) {
		node = AnimationBuilder::Instance()->Create(anim);
	}
	// particle3d
	else if (const eparticle3d::Symbol* p3d = dynamic_cast<const eparticle3d::Symbol*>(sym)) {
		node = Particle3dBuilder::Instance()->Create(p3d);
	}
	// particle2d
	else if (const eparticle2d::Symbol* p2d = dynamic_cast<const eparticle2d::Symbol*>(sym)) {
		node = Particle2dBuilder::Instance()->Create(p2d);
	}
	// trail
	else if (const etrail::Symbol* trail = dynamic_cast<const etrail::Symbol*>(sym)) {
		node = TrailBuilder::Instance()->Create(trail);
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

	builders.push_back(ImageBuilder::Instance());

	builders.push_back(Scale9Builder::Instance());
	builders.push_back(Scale9SprBuilder::Instance());

	builders.push_back(IconBuilder::Instance());
	builders.push_back(IconSprBuilder::Instance());

	builders.push_back(TextureBuilder::Instance());

	builders.push_back(LabelBuilder::Instance());

	builders.push_back(ComplexBuilder::Instance());
	builders.push_back(ComplexSprBuilder::Instance());

	builders.push_back(AnimationBuilder::Instance());
	builders.push_back(AnimationSprBuilder::Instance());

	builders.push_back(Particle3dBuilder::Instance());
	builders.push_back(Particle3dSprBuilder::Instance());

	builders.push_back(Particle2dBuilder::Instance());
	builders.push_back(Particle2dSprBuilder::Instance());

	builders.push_back(TrailBuilder::Instance());
	builders.push_back(TrailSprBuilder::Instance());
}

}