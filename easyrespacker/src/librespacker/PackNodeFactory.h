#ifndef _EASYRESPACKER_PACK_NODE_FACTORY_H_
#define _EASYRESPACKER_PACK_NODE_FACTORY_H_

#include "ExportNameSet.h"

#include <vector>

namespace erespacker
{

class IPackNode;

class INodeBuilder;

// picture
class ImageBuilder;
class Scale9Builder;
class IconBuilder;
class TextureBuilder;

// label
class TextBuilder;

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

// p3d spr
class P3dSprBuilder;

// clipbox
class ClipboxBuilder;

// shape
class ShapeBuilder;

// mesh
class MeshBuilder;

// mesh spr
class MeshSprBuilder;

// mask
class MaskBuilder;

// trail
class TrailBuilder;

class PackNodeFactory
{
public:
	const IPackNode* Create(const ee::Sprite* spr);	
	const IPackNode* Create(const ee::Symbol* sym);	

	void GetAllNodes(std::vector<IPackNode*>& nodes) const;

	void SetFilesDir(const std::string& files_dir) {
		m_files_dir = files_dir;
	}

	const std::map<std::string, int>& GetExportSet() const { 
		return m_export_set.GetData(); 
	}

	const TextBuilder* GetTextBuilder() const { return m_text_builder; }
	TextBuilder* GetTextBuilder() { return m_text_builder; }

	void Release();

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
	TextBuilder* m_text_builder;

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

	// p3d spr
	P3dSprBuilder* m_p3d_spr_builder;

	// clipbox
	ClipboxBuilder* m_clipbox_builder;

	// shape
	ShapeBuilder* m_shape_builder;

	// mesh
	MeshBuilder* m_mesh_builder;

	// mesh spr
	MeshSprBuilder* m_mesh_spr_builder;

	// mask
	MaskBuilder* m_mask_builder;

	// trail
	TrailBuilder* m_trail_builder;

private:
	static PackNodeFactory* m_instance;

}; // PackNodeFactory

}

#endif // _EASYRESPACKER_PACK_NODE_FACTORY_H_