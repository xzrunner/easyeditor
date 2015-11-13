#ifndef _LIBRESPACKER_PACK_NODE_FACTORY_H_
#define _LIBRESPACKER_PACK_NODE_FACTORY_H_

#include "ExportNameSet.h"

namespace librespacker
{

class IPackNode;

class INodeBuilder;

// picture
class ImageBuilder;
class Scale9Builder;
class IconBuilder;
class TextureBuilder;

// label
class LabelBuilder;

// animation
class ComplexBuilder;
class AnimBuilder;
class Terrain2DBuilder;

// anchor
class AnchorBuilder;

// particle3d
class Particle3DBuilder;

// particle2d
class Particle2DBuilder;

// clipbox
class ClipboxBuilder;

class PackNodeFactory
{
public:
	const IPackNode* Create(const d2d::ISprite* spr);	
	const IPackNode* Create(const d2d::ISymbol* symbol);	

	void GetAllNodes(std::vector<IPackNode*>& nodes) const;

	void SetFilesDir(const std::string& files_dir) {
		m_files_dir = files_dir;
	}

	const std::map<std::string, int>& GetExportSet() const { 
		return m_export_set.GetData(); 
	}

	const LabelBuilder* GetLabelBuilder() const { return m_label_builder; }

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

	// anchor
	AnchorBuilder* m_anchor_builder;

	// particle3d
	Particle3DBuilder* m_particle3d_builder;

	// particle2d
	Particle2DBuilder* m_particle2d_builder;

	// clipbox
	ClipboxBuilder* m_clipbox_builder;

private:
	static PackNodeFactory* m_instance;

}; // PackNodeFactory

}

#endif // _LIBRESPACKER_PACK_NODE_FACTORY_H_