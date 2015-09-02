#ifndef _LIBCOCO_PACK_NODE_FACTORY_H_
#define _LIBCOCO_PACK_NODE_FACTORY_H_

#include "ExportNameSet.h"

namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }
namespace ebuilder { class CodeGenerator; }

namespace libcoco
{

class TexturePacker;

class IPackNode;

class INodePacker;

class ImagePacker;
class Scale9Packer;
class IconPacker;
class ETexturePacker;

class LabelPacker;

class ComplexPacker;
class AnimPacker;

class PackNodeFactory
{
public:
	const IPackNode* Create(const d2d::ISprite* spr);	

	void CreateComplex(const ecomplex::Symbol* complex);
	void CreateAnim(const libanim::Symbol* anim);

	void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	static PackNodeFactory* Instance();

private:
	PackNodeFactory();

private:
	ExportNameSet m_export_set;

	// picture
	ImagePacker* m_img_packer;
	Scale9Packer* m_scale9_packer;
	IconPacker* m_icon_packer;
	ETexturePacker* m_tex_packer;

	// label
	LabelPacker* m_label_packer;

	// animation
	ComplexPacker* m_complex_packer;
	AnimPacker* m_anim_packer;

	std::vector<INodePacker*> m_packers;

private:
	static PackNodeFactory* m_instance;

}; // PackNodeFactory

}

#endif // _LIBCOCO_PACK_NODE_FACTORY_H_