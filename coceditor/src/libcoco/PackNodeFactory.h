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

class INodeBuilder;

class ImageBuilder;
class Scale9Builder;
class IconBuilder;
class TextureBuilder;

class LabelBuilder;

class ComplexBuilder;
class AnimBuilder;

class PackNodeFactory
{
public:
	const IPackNode* Create(const d2d::ISprite* spr);	

	void CreateComplex(const ecomplex::Symbol* complex);
	void CreateAnim(const libanim::Symbol* anim);

	void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const;

	void SetFilesDir(const std::string& files_dir) {
		m_files_dir = files_dir;
	}

	static PackNodeFactory* Instance();

private:
	PackNodeFactory();

private:
	std::string m_files_dir;

	ExportNameSet m_export_set;

	// picture
	ImageBuilder* m_img_packer;
	Scale9Builder* m_scale9_packer;
	IconBuilder* m_icon_packer;
	TextureBuilder* m_tex_packer;

	// label
	LabelBuilder* m_label_packer;

	// animation
	ComplexBuilder* m_complex_packer;
	AnimBuilder* m_anim_packer;

	std::vector<INodeBuilder*> m_packers;

private:
	static PackNodeFactory* m_instance;

}; // PackNodeFactory

}

#endif // _LIBCOCO_PACK_NODE_FACTORY_H_