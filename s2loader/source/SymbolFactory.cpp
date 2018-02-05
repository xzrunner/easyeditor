#include "s2loader/SymbolFactory.h"
#include "s2loader/SymbolFile.h"

#include "s2loader/ImageSymLoader.h"
#include "s2loader/Scale9SymLoader.h"
#include "s2loader/IconSymLoader.h"
#include "s2loader/TextureSymLoader.h"
#include "s2loader/TextboxLoader.h"
#include "s2loader/ComplexSymLoader.h"
#include "s2loader/AnimSymLoader.h"
#include "s2loader/Anim2SymLoader.h"
#include "s2loader/P3dSymLoader.h"
#include "s2loader/P2dSymLoader.h"
#include "s2loader/ShapeSymLoader.h"
#include "s2loader/MeshSymLoader.h"
#include "s2loader/MaskSymLoader.h"
#include "s2loader/TrailSymLoader.h"
#include "s2loader/SkeletonSymLoader.h"
#include "s2loader/ModelSymLoader.h"

#include <logger.h>
#include <simp/NodeFactory.h>
#include <simp/simp_types.h>
#include <simp/NodePicture.h>
#include <timp/PkgMgr.h>
#include <timp/Package.h>
#include <guard/check.h>
#include <gum/FilepathHelper.h>
#include <gum/ImageSymbol.h>
#include <gum/ImagePool.h>
#include <gum/TextboxSymbol.h>
#include <gum/ProxyImage.h>

#include <sprite2/AnchorSymbol.h>
#include <sprite2/Scale9Symbol.h>
#include <sprite2/IconSymbol.h>
#include <sprite2/TextureSymbol.h>
#include <sprite2/ComplexSymbol.h>
#include <sprite2/AnimSymbol.h>
#include <sprite2/Anim2Symbol.h>
#include <sprite2/Particle3dSymbol.h>
#include <sprite2/Particle2dSymbol.h>
#include <sprite2/ShapeSymbol.h>
#include <sprite2/MeshSymbol.h>
#include <sprite2/MaskSymbol.h>
#include <sprite2/TrailSymbol.h>
#include <sprite2/SkeletonSymbol.h>
#include <sprite2/ModelSymbol.h>
#include <sprite2/AudioSymbol.h>
#include <sprite2/SymType.h>
#include <sprite2/CacheMatVisitor.h>

#include <fstream>

#include <assert.h>

namespace s2loader
{

CU_SINGLETON_DEFINITION(SymbolFactory);

SymbolFactory::SymbolFactory()
{
}

s2::SymPtr SymbolFactory::Create(const CU_STR& filepath, int type) const
{
	s2::SymPtr ret = nullptr;

	auto ext = gum::FilepathHelper::Extension(filepath);
	enum FileType
	{
		FILE_UNKNOWN = 0,
		FILE_JSON,
		FILE_BIN,
	};
	FileType ft = FILE_UNKNOWN;
	if (ext == ".json") {
		ft = FILE_JSON;
	} else if (ext == ".bin") {
		ft = FILE_BIN;
	}

	if (type == s2::SYM_UNKNOWN) {
		type = SymbolFile::Instance()->Type(filepath);
	}
	switch (type)
	{
	case s2::SYM_IMAGE:
		{
			auto sym = CU_MAKE_SHARED<gum::ImageSymbol>();
			ImageSymLoader loader(*sym);
			loader.Load(bimp::FilePath(filepath), 1, false);
			ret = sym;
		}
		break;
	case s2::SYM_SCALE9:
		{
			auto sym = CU_MAKE_SHARED<s2::Scale9Symbol>();
			Scale9SymLoader loader(*sym);
			switch (ft)
			{
			case FILE_JSON:
				loader.LoadJson(filepath);
				break;
			case FILE_BIN:
				loader.LoadSns(filepath);
				break;
			}
			ret = sym;
		}
		break;
	case s2::SYM_ICON:
		{
			auto sym = CU_MAKE_SHARED<s2::IconSymbol>();
			IconSymLoader loader(*sym);
			switch (ft)
			{
			case FILE_JSON:
				loader.LoadJson(filepath);
				break;
			case FILE_BIN:
				loader.LoadSns(filepath);
				break;
			}
			ret = sym;
		}
		break;
	case s2::SYM_TEXTURE:
		{
			auto sym = CU_MAKE_SHARED<s2::TextureSymbol>();
			TextureSymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_TEXTBOX:
		{
			auto sym = CU_MAKE_SHARED<gum::TextboxSymbol>();
			TextboxLoader loader(sym->GetTextbox());
			Json::Value val;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.c_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, val);
			fin.close();
			loader.LoadJson(val);
			ret = sym;
		}
		break;
	case s2::SYM_COMPLEX:
		{
			auto sym = CU_MAKE_SHARED<s2::ComplexSymbol>();
			ComplexSymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_ANIMATION:
		{
			auto sym = CU_MAKE_SHARED<s2::AnimSymbol>();
			AnimSymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_ANIM2:
		{
			auto sym = CU_MAKE_SHARED<s2::Anim2Symbol>();
			Anim2SymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_PARTICLE3D:
		{
			auto sym = CU_MAKE_SHARED<s2::Particle3dSymbol>();
			P3dSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case s2::SYM_PARTICLE2D:
		{
			auto sym = CU_MAKE_SHARED<s2::Particle2dSymbol>();
			P2dSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case s2::SYM_SHAPE:
		{
			auto sym = CU_MAKE_SHARED<s2::ShapeSymbol>();
			ShapeSymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_MESH:
		{
			auto sym = CU_MAKE_SHARED<s2::MeshSymbol>();
			MeshSymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_MASK:
		{
			auto sym = CU_MAKE_SHARED<s2::MaskSymbol>();
			MaskSymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_TRAIL:
		{
			auto sym = CU_MAKE_SHARED<s2::TrailSymbol>();
			TrailSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case s2::SYM_SKELETON:
		{
			auto sym = CU_MAKE_SHARED<s2::SkeletonSymbol>();
			SkeletonSymLoader loader(*sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_MODEL:
		{
			auto sym = CU_MAKE_SHARED<s2::ModelSymbol>();
			ModelSymLoader loader(*sym);
			loader.LoadFromFile(filepath);
			ret = sym;
		}
		break;
	case s2::SYM_AUDIO:
		{
			auto sym = CU_MAKE_SHARED<s2::AudioSymbol>();
			ret = sym;
		}
		break;
	default:
		LOGW("Error Sym Type: filepath %s, type %d", filepath.c_str(), type);
	}

	if (!ret) {
		LOGW("Create sym fail: filepath %s", filepath.c_str());
	}

	return ret;
}

s2::SymPtr SymbolFactory::Create(uint32_t id) const
{
	if (id == 0xffffffff) {
		if (!m_anchor_sym) {
			m_anchor_sym = CU_MAKE_SHARED<s2::AnchorSymbol>();
		}
		return m_anchor_sym;
	}

	s2::SymPtr ret = nullptr;

	int type;
	const void* data = simp::NodeFactory::Instance()->Create(id, &type);
	if(!data) {
		LOGW("Create sym fail: id %u", id);
		return nullptr;
	}
	switch (type)
	{
	case simp::TYPE_IMAGE:
		{
			const simp::NodePicture* pic = (const simp::NodePicture*)data;
			sm::vec2 offset(pic->offx * 0.5f, pic->offy * 0.5f);
			std::shared_ptr<gum::ImageSymbol> sym = nullptr;
			if (pic->texid < simp::NodePicture::MAX_IN_PKG)
			{
				int pkg_id = simp::NodeID::GetPkgID(id);
				const timp::Package* t_pkg = timp::PkgMgr::Instance()->Query(pkg_id);

				const bimp::FilePath& fp = t_pkg->GetTexPath(pic->texid, 0);

				sym = CU_MAKE_SHARED<gum::ImageSymbol>(id);
				ImageSymLoader loader(*sym);

				bool async = true;
				const simp::Package* s_pkg = simp::NodeFactory::Instance()->QueryPkg(pkg_id);
				loader.Load(fp, s_pkg->GetScale(), async);
				const timp::Package::TextureDesc& tex = t_pkg->GetTexDesc(pic->texid);
				auto& img = sym->GetImage();
				if (img && async) {
					img->AsyncLoad(pkg_id, tex.type, tex.w, tex.h);
				}

				sym->SetRegion(
					sm::ivec2(pic->region[0], pic->region[1]), 
					sm::ivec2(pic->region[2], pic->region[3]),
					offset,
					pic->lod,
					s_pkg->GetScale());
			}
			else
			{
				CU_STR filepath = gum::ProxyImage::GetFilepath(pic->texid - simp::NodePicture::MAX_IN_PKG);
				auto img = gum::ImagePool::Instance()->Query(bimp::FilePath(filepath));
				assert(img);

				int pkg_id = simp::NodeID::GetPkgID(id);
				const simp::Package* s_pkg = simp::NodeFactory::Instance()->QueryPkg(pkg_id);

				sym = CU_MAKE_SHARED<gum::ImageSymbol>(id);
				sym->SetImage(img);
				sym->SetRegion(
					sm::ivec2(pic->region[0], pic->region[1]), 
					sm::ivec2(pic->region[2], pic->region[3]),
					offset,
					pic->lod,
					s_pkg->GetScale());
			}
			ret = sym;
		}
		break;
	case simp::TYPE_SCALE9:
		{
			auto sym = CU_MAKE_SHARED<s2::Scale9Symbol>(id);
			Scale9SymLoader loader(*sym);
			loader.LoadBin((const simp::NodeScale9*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_ICON:
		{
			auto sym = CU_MAKE_SHARED<s2::IconSymbol>(id);
			IconSymLoader loader(*sym);
			loader.LoadBin((const simp::NodeIcon*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_TEXTURE:
		{
			auto sym = CU_MAKE_SHARED<s2::TextureSymbol>(id);
			TextureSymLoader loader(*sym);
			loader.LoadBin((const simp::NodeTexture*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_LABEL:
		{
			auto sym = CU_MAKE_SHARED<gum::TextboxSymbol>(id);
			TextboxLoader loader(sym->GetTextbox());
			loader.LoadBin((const simp::NodeLabel*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_COMPLEX:
		{
			auto sym = CU_MAKE_SHARED<s2::ComplexSymbol>(id);
			ComplexSymLoader loader(*sym);
			loader.LoadBin((const simp::NodeComplex*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_ANIMATION:
		{
			auto sym = CU_MAKE_SHARED<s2::AnimSymbol>(id);
			AnimSymLoader loader(*sym, nullptr, nullptr);
			loader.LoadBin((const simp::NodeAnimation*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_ANIM2:
		{
			auto sym = CU_MAKE_SHARED<s2::Anim2Symbol>(id);
			Anim2SymLoader loader(*sym);
			loader.LoadBin((const simp::NodeAnim2*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_PARTICLE3D:
		{
			auto sym = CU_MAKE_SHARED<s2::Particle3dSymbol>(id);
			P3dSymLoader loader;
			loader.LoadBin((const simp::NodeParticle3d*)data);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case simp::TYPE_PARTICLE2D:
		{
			auto sym = CU_MAKE_SHARED<s2::Particle2dSymbol>(id);
			P2dSymLoader loader;
			loader.LoadBin((const simp::NodeParticle2d*)data);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case simp::TYPE_SHAPE:
		{
			auto sym = CU_MAKE_SHARED<s2::ShapeSymbol>(id);
			ShapeSymLoader loader(*sym);
			loader.LoadBin((const simp::NodeShape*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_MESH:
		{
			auto sym = CU_MAKE_SHARED<s2::MeshSymbol>(id);
			MeshSymLoader loader(*sym);
			loader.LoadBin((const simp::NodeMesh*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_MASK:
		{
			auto sym = CU_MAKE_SHARED<s2::MaskSymbol>(id);
			MaskSymLoader loader(*sym);
			loader.LoadBin((const simp::NodeMask*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_TRAIL:
		{
			auto sym = CU_MAKE_SHARED<s2::TrailSymbol>(id);
			TrailSymLoader loader;
			loader.LoadBin((const simp::NodeTrail*)data);
			loader.Store(sym);
			ret = sym;
		}
		break;
	default:
		LOGW("Create sym fail: id %u, type %d", id, type);
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
 	if (ret && type != simp::TYPE_SCALE9 && type != simp::TYPE_ANIMATION) {
 		ret->Traverse(s2::CacheMatVisitor());
 	}
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	if (!ret) {
		LOGW("Create sym fail: id %u", id);
	}

	return ret;
}

s2::SymPtr SymbolFactory::Create(const CU_STR& pkg_name, const CU_STR& node_name) const
{
	uint32_t id = simp::NodeFactory::Instance()->GetNodeID(pkg_name, node_name);
	if (id != 0xffffffff) {
		return Create(id);
	} else {
		LOGW("Create sym fail: pkg %s, node %s", 
			pkg_name.c_str(), node_name.c_str());
		return nullptr;
	}
}

}