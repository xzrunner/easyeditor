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

#include <easyscale9.h>
#include <easyicon.h>
#include <easytexture.h>
#include <easytext.h>
#include <easycomplex.h>
#include <easyanim.h>

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

	else {
		throw ee::Exception("PackNodeFactory::Create unknown sprite type.");
	}

	node->SetFilepath(ee::FileHelper::GetRelativePath(m_files_dir, dynamic_cast<const  ee::Symbol*>(spr->GetSymbol())->GetFilepath()));

	return node;
}

const PackNode* PackNodeFactory::Create(const ee::Symbol* sym)
{
	const PackNode* node = NULL;

	// scale9
	if (const escale9::Symbol* s9 = dynamic_cast<const escale9::Symbol*>(sym)) {
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
}

}