#include "PackNodeFactory.h"

#include "ImageBuilder.h"

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

	else {
		throw ee::Exception("PackNodeFactory::Create unknown sprite type.");
	}

	node->SetFilepath(ee::FileHelper::GetRelativePath(m_files_dir, dynamic_cast<const  ee::Symbol*>(spr->GetSymbol())->GetFilepath()));

	return node;
}

const PackNode* PackNodeFactory::Create(const ee::Symbol* sym)
{
	return NULL;
}

}