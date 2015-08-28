#ifndef _LIBCOCO_PACK_NODE_FACTORY_H_
#define _LIBCOCO_PACK_NODE_FACTORY_H_

#include "ExportNameSet.h"

namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }

namespace libcoco
{

class IPackNode;

class ImageBuilder;
class Scale9Builder;
class IconBuilder;

class LabelBuilder;

class ComplexBuilder;
class AnimBuilder;

class PackNodeFactory
{
public:
	const IPackNode* Create(const d2d::ISprite* spr);	

	void CreateComplex(const ecomplex::Symbol* complex);
	void CreateAnim(const libanim::Symbol* anim);

	static PackNodeFactory* Instance();

private:
	PackNodeFactory();

private:
	ExportNameSet m_export_set;

	// picture
	ImageBuilder* m_img_builder;
	Scale9Builder* m_scale9_builder;
	IconBuilder* m_icon_builder;

	// label
	LabelBuilder* m_label_builder;

	// animation
	ComplexBuilder* m_complex_builder;
	AnimBuilder* m_anim_builder;

private:
	static PackNodeFactory* m_instance;

}; // PackNodeFactory

}

#endif // _LIBCOCO_PACK_NODE_FACTORY_H_