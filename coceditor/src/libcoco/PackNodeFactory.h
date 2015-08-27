#ifndef _LIBCOCO_PACK_NODE_FACTORY_H_
#define _LIBCOCO_PACK_NODE_FACTORY_H_

#include <drag2d.h>

namespace libcoco
{

class IPackNode;

class ImageBuilder;
class Scale9Builder;
class LabelBuilder;
class ComplexBuilder;
class AnimBuilder;
class IconBuilder;

class PackNodeFactory
{
public:
	const IPackNode* Create(const d2d::ISprite* spr);	

	static PackNodeFactory* Instance();

private:
	PackNodeFactory();

private:
	// picture
	ImageBuilder* m_img_builder;
	Scale9Builder* m_scale9_builder;

	// label
	LabelBuilder* m_label_builder;

	// animation
	ComplexBuilder* m_complex_builder;
	AnimBuilder* m_anim_builder;
	IconBuilder* m_icon_builder;

private:
	static PackNodeFactory* m_instance;

}; // PackNodeFactory

}

#endif // _LIBCOCO_PACK_NODE_FACTORY_H_