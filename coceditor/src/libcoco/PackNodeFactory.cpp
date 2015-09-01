#include "PackNodeFactory.h"

#include "ImagePacker.h"
#include "Scale9Packer.h"
#include "IconPacker.h"
#include "ETexturePacker.h"

#include "LabelPacker.h"

#include "ComplexPacker.h"
#include "AnimPacker.h"

namespace libcoco
{

PackNodeFactory* PackNodeFactory::m_instance = NULL;

PackNodeFactory::PackNodeFactory()
{
	m_packers.push_back(m_img_packer = new ImagePacker);
	m_packers.push_back(m_scale9_packer = new Scale9Packer);
	m_packers.push_back(m_icon_packer = new IconPacker);
	m_packers.push_back(m_tex_packer = new ETexturePacker);

	m_packers.push_back(m_label_packer = new LabelPacker);

	m_packers.push_back(m_complex_packer = new ComplexPacker(m_export_set));
	m_packers.push_back(m_anim_packer = new AnimPacker(m_export_set));
}

const IPackNode* PackNodeFactory::Create(const d2d::ISprite* spr)
{
	const IPackNode* node = NULL;
	
	if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(spr)) {
		node = m_img_packer->Create(image);
	} else if (const escale9::Sprite* scale9 = dynamic_cast<const escale9::Sprite*>(spr)) {
		node = m_scale9_packer->Create(scale9);
	} else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) {
		node = m_icon_packer->Create(icon);
	} else if (const etexture::Sprite* tex = dynamic_cast<const etexture::Sprite*>(spr)) {
		node = m_tex_packer->Create(&tex->GetSymbol());
	}

	else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(spr)) {
		node = m_label_packer->Create(font);
	} 

	else if (const ecomplex::Sprite* complex = dynamic_cast<const ecomplex::Sprite*>(spr)) {
		node = m_complex_packer->Create(&complex->GetSymbol());
	} else if (const libanim::Sprite* anim = dynamic_cast<const libanim::Sprite*>(spr)) {
		node = m_anim_packer->Create(&anim->GetSymbol());
	}

	return node;
}

void PackNodeFactory::CreateComplex(const ecomplex::Symbol* complex)
{
	m_complex_packer->Create(complex);
}

void PackNodeFactory::CreateAnim(const libanim::Symbol* anim)
{
	m_anim_packer->Create(anim);
}

void PackNodeFactory::PackToLuaString(ebuilder::CodeGenerator& gen,
							   const TexturePacker& tp) const
{
	for (int i = 0, n = m_packers.size(); i < n; ++i) {
		m_packers[i]->PackToLuaString(gen, tp);
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