#ifndef _LIBCOCO_PACK_NODE_FACTORY_H_
#define _LIBCOCO_PACK_NODE_FACTORY_H_

#include "ExportNameSet.h"

namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }
namespace ebuilder { class CodeGenerator; }

namespace libcoco
{

class IPackNode;

class INodeBuilder;

class ImageBuilder;
class Scale9Builder;
class IconBuilder;
class TextureBuilder;

class LabelBuilder;

class ComplexBuilder;
class AnimBuilder;
class Terrain2DBuilder;

class PackNodeFactory
{
public:
	const IPackNode* Create(const d2d::ISprite* spr);	

	void CreateComplex(const ecomplex::Symbol* complex);
	void CreateAnim(const libanim::Symbol* anim);

	void GetAllNodes(std::vector<IPackNode*>& nodes) const;

	void SetFilesDir(const std::string& files_dir) {
		m_files_dir = files_dir;
	}

	const std::map<std::string, int>& GetExportSet() const { 
		return m_export_set.GetData(); 
	}

	static PackNodeFactory* Instance();

private:
	PackNodeFactory();

private:
	std::string m_files_dir;

	ExportNameSet m_export_set;

	std::vector<INodeBuilder*> m_builders;

private:
	// picture
	ImageBuilder* m_img_builder;
	Scale9Builder* m_scale9_builder;
	IconBuilder* m_icon_builder;
	TextureBuilder* m_tex_builder;

	// label
	LabelBuilder* m_label_builder;

	// animation
	ComplexBuilder* m_complex_builder;
	AnimBuilder* m_anim_builder;
	Terrain2DBuilder* m_terrain2d_builder;

private:
	static PackNodeFactory* m_instance;

}; // PackNodeFactory

}

#endif // _LIBCOCO_PACK_NODE_FACTORY_H_