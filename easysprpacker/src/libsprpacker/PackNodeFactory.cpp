#include "PackNodeFactory.h"

#include "ImageBuilder.h"
#include "Scale9Builder.h"
#include "Scale9SprBuilder.h"
#include "IconBuilder.h"
#include "IconSprBuilder.h"
#include "TextureBuilder.h"
#include "LabelBuilder.h"

#include <easyscale9.h>
#include <easyicon.h>
#include <easytexture.h>
#include <easytext.h>

#include <ee/Sprite.h>
#include <ee/ImageSprite.h>
#include <ee/ImageSymbol.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>

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

	else {
		throw ee::Exception("PackNodeFactory::Create unknown symbol type.");
	}

	node->SetFilepath(ee::FileHelper::GetRelativePath(m_files_dir, sym->GetFilepath()));

	return node;
}

}