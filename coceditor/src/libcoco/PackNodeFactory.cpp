#include "PackNodeFactory.h"

#include "ImageBuilder.h"
#include "Scale9Builder.h"

#include "LabelBuilder.h"

#include "ComplexBuilder.h"
#include "AnimBuilder.h"
#include "IconBuilder.h"

namespace libcoco
{

PackNodeFactory* PackNodeFactory::m_instance = NULL;

PackNodeFactory::PackNodeFactory()
{
	m_img_builder = new ImageBuilder;
	m_scale9_builder = new Scale9Builder;

	m_label_builder = new LabelBuilder;

	m_complex_builder = new ComplexBuilder;
	m_anim_builder = new AnimBuilder;
	m_icon_builder = new IconBuilder;
}

const IPackNode* PackNodeFactory::Create(const d2d::ISprite* spr)
{
	const IPackNode* node = NULL;
	if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(spr)) {
		node = m_img_builder->Create(image);
	} else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(spr)) {
		node = m_label_builder->Create(font);
	} 
	
// 	else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) {
// 
// 	}

	return node;
}

PackNodeFactory* PackNodeFactory::Instance()
{
	if (!m_instance) {
		m_instance = new PackNodeFactory;
	}
	return m_instance;
}

}