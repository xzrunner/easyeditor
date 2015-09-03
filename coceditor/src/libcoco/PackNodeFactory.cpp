#include "PackNodeFactory.h"

#include "ImageBuilder.h"
#include "Scale9Builder.h"
#include "IconBuilder.h"
#include "TextureBuilder.h"

#include "LabelBuilder.h"

#include "ComplexBuilder.h"
#include "AnimBuilder.h"

namespace libcoco
{

PackNodeFactory* PackNodeFactory::m_instance = NULL;

PackNodeFactory::PackNodeFactory()
{
	m_builders.push_back(m_img_builder = new ImageBuilder);
	m_builders.push_back(m_scale9_builder = new Scale9Builder);
	m_builders.push_back(m_icon_builder = new IconBuilder);
	m_builders.push_back(m_tex_builder = new TextureBuilder);

	m_builders.push_back(m_label_builder = new LabelBuilder);

	m_builders.push_back(m_complex_builder = new ComplexBuilder(m_export_set));
	m_builders.push_back(m_anim_builder = new AnimBuilder(m_export_set));
}

const IPackNode* PackNodeFactory::Create(const d2d::ISprite* spr)
{
	const IPackNode* node = NULL;
	
	if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(spr)) {
		node = m_img_builder->Create(image);
	} else if (const escale9::Sprite* scale9 = dynamic_cast<const escale9::Sprite*>(spr)) {
		node = m_scale9_builder->Create(scale9);
	} else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) {
		node = m_icon_builder->Create(icon);
	} else if (const etexture::Sprite* tex = dynamic_cast<const etexture::Sprite*>(spr)) {
		node = m_tex_builder->Create(&tex->GetSymbol());
	}

	else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(spr)) {
		node = m_label_builder->Create(font);
	} 

	else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		node = m_complex_builder->Create(&complex->GetSymbol());
	} else if (const libanim::Sprite* anim = dynamic_cast<const libanim::Sprite*>(spr)) {
		node = m_anim_builder->Create(&anim->GetSymbol());
	}

	node->SetFilepath(d2d::FilenameTools::getRelativePath(m_files_dir, spr->GetSymbol().GetFilepath()).ToStdString());

	return node;
}

void PackNodeFactory::CreateComplex(const ecomplex::Symbol* complex)
{
	const IPackNode* node = m_complex_builder->Create(complex);
	node->SetFilepath(d2d::FilenameTools::getRelativePath(m_files_dir, complex->GetFilepath()).ToStdString());
}

void PackNodeFactory::CreateAnim(const libanim::Symbol* anim)
{
	const IPackNode* node = m_anim_builder->Create(anim);
	node->SetFilepath(d2d::FilenameTools::getRelativePath(m_files_dir, anim->GetFilepath()).ToStdString());
}

void PackNodeFactory::PackToLuaString(ebuilder::CodeGenerator& gen,
									  const d2d::TexturePacker& tp) const
{
	std::vector<IPackNode*> nodes;
	for (int i = 0, n = m_builders.size(); i < n; ++i) {
		m_builders[i]->Traverse(d2d::FetchAllVisitor<IPackNode>(nodes));
	}

	std::sort(nodes.begin(), nodes.end(), PackNodeCmp());
 	for (int i = 0, n = nodes.size(); i < n; ++i) {
 		nodes[i]->PackToLuaString(gen, tp);
 	}
}

PackNodeFactory* PackNodeFactory::Instance()
{
	if (!m_instance) {
		m_instance = new PackNodeFactory;
	}
	return m_instance;
}

}