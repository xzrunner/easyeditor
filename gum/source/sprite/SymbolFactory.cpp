#include "SymbolFactory.h"
#include "StringHelper.h"
#include "SymFileType.h"

#include "ImageSymbol.h"
#include "ImageSymLoader.h"
#include "Scale9SymLoader.h"
#include "TextureSymLoader.h"
#include "TextboxSymbol.h"
#include "TextboxLoader.h"
#include "ComplexSymLoader.h"
#include "AnimSymLoader.h"
#include "P3dSymLoader.h"
#include "P2dSymLoader.h"
#include "MeshSymLoader.h"
#include "MaskSymLoader.h"
#include "TrailSymLoader.h"

#include <simp/NodeFactory.h>
#include <simp/Package.h>
#include <simp/simp_types.h>

#include <simp/NodePicture.h>

#include <sprite2/Scale9Symbol.h>
#include <sprite2/TextureSymbol.h>
#include <sprite2/ComplexSymbol.h>
#include <sprite2/AnimSymbol.h>
#include <sprite2/Particle3dSymbol.h>
#include <sprite2/Particle2dSymbol.h>
#include <sprite2/MeshSymbol.h>
#include <sprite2/MaskSymbol.h>
#include <sprite2/TrailSymbol.h>

#include <assert.h>

namespace gum
{

SINGLETON_DEFINITION(SymbolFactory);

SymbolFactory::SymbolFactory()
{
}

SymbolFactory::~SymbolFactory()
{
	std::map<std::string, s2::Symbol*>::iterator itr = m_path_cache.begin();
	for ( ; itr != m_path_cache.end(); ++itr) {
		itr->second->RemoveReference();
	}

	std::map<uint32_t, s2::Symbol*>::iterator itr2 = m_id_cache.begin();
	for ( ; itr2 != m_id_cache.end(); ++itr2) {
		itr2->second->RemoveReference();
	}
}

s2::Symbol* SymbolFactory::Create(const std::string& filepath, SymFileType* _type) const
{
	std::string fixed_path = filepath;
	StringHelper::ToLower(fixed_path);

	SymFileType type = get_sym_file_type(fixed_path);
	if (_type) {
		*_type = type;
	}

	std::map<std::string, s2::Symbol*>::const_iterator itr 
		= m_path_cache.find(fixed_path);
	if (itr != m_path_cache.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	s2::Symbol* ret = NULL;
	switch (type)
	{
	case IMAGE:
		{
			ImageSymbol* sym = new ImageSymbol;
			ImageSymLoader loader(sym);
			loader.Load(filepath);
			ret = sym;
		}
		break;
	case SCALE9:
		{
			s2::Scale9Symbol* sym = new s2::Scale9Symbol();
			Scale9SymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case TEXTURE:
		{
			s2::TextureSymbol* sym = new s2::TextureSymbol;
			TextureSymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case TEXTBOX:
		{
			TextboxSymbol* sym = new TextboxSymbol();
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
	case COMPLEX:
		{
			s2::ComplexSymbol* sym = new s2::ComplexSymbol();
			ComplexSymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case ANIMATION:
		{
			s2::AnimSymbol* sym = new s2::AnimSymbol();
			AnimSymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case PARTICLE3D:
		{
			s2::Particle3dSymbol* sym = new s2::Particle3dSymbol;
			P3dSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case PARTICLE2D:
		{
			s2::Particle2dSymbol* sym = new s2::Particle2dSymbol;
			P2dSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	case MESH:
		{
			s2::MeshSymbol* sym = new s2::MeshSymbol;
			MeshSymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case MASK:
		{
			s2::MaskSymbol* sym = new s2::MaskSymbol;
			MaskSymLoader loader(sym);
			loader.LoadJson(filepath);
			ret = sym;
		}
		break;
	case TRAIL:
		{
			s2::TrailSymbol* sym = new s2::TrailSymbol;
			TrailSymLoader loader;
			loader.LoadJson(filepath);
			loader.Store(sym);
			ret = sym;
		}
		break;
	default:
		assert(0);
	}

	if (ret) {
		m_path_cache.insert(std::make_pair(fixed_path, ret));
		ret->AddReference();
	}

	return ret;
}

s2::Symbol* SymbolFactory::Create(uint32_t id, SymFileType* _type) const
{
	std::map<uint32_t, s2::Symbol*>::const_iterator itr = m_id_cache.find(id);
	if (itr != m_id_cache.end()) {
		itr->second->AddReference();
		return itr->second;
	}

	s2::Symbol* ret = NULL;

	SymFileType stype = UNKNOWN;

	int type;
	const void* data = simp::NodeFactory::Instance()->Create(id, &type);
	assert(data);
	switch (type)
	{
	case simp::TYPE_IMAGE:
		{
			stype = IMAGE;

			const simp::NodePicture* pic = (const simp::NodePicture*)data;

			const simp::Package* pkg = simp::NodeFactory::Instance()->GetPkg(id);
			std::string filepath = pkg->GetImagePath(pic->texid);
			
			ImageSymbol* sym = new ImageSymbol;
			ImageSymLoader loader(sym);
			loader.Load(filepath);
			sym->SetRegion(
				sm::ivec2(pic->region[0], pic->region[1]), 
				sm::ivec2(pic->region[2], pic->region[3]));
			ret = sym;
		}
		break;
	case simp::TYPE_SCALE9:
		{
			stype = SCALE9;

			s2::Scale9Symbol* sym = new s2::Scale9Symbol;
			Scale9SymLoader loader(sym);
			loader.LoadBin((const simp::NodeScale9*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_LABEL:
		{
			stype = TEXTBOX;

			TextboxSymbol* sym = new TextboxSymbol();
			TextboxLoader loader(sym->GetTextbox());
			loader.LoadBin((const simp::NodeLabel*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_COMPLEX:
		{
			stype = COMPLEX;

			s2::ComplexSymbol* sym = new s2::ComplexSymbol;
			ComplexSymLoader loader(sym);
			loader.LoadBin((const simp::NodeComplex*)data);
			ret = sym;
		}
		break;
	case simp::TYPE_ANIMATION:
		{
			stype = ANIMATION;

			s2::AnimSymbol* sym = new s2::AnimSymbol;
			AnimSymLoader loader(sym);
			loader.LoadBin((const simp::NodeAnimation*)data);
			ret = sym;
		}
		break;
	default:
		assert(0);
	}	

	if (_type) {
		*_type = stype;
	}

	return ret;
}

s2::Symbol* SymbolFactory::Create(const std::string& pkg_name, const std::string& node_name, SymFileType* type) const
{
	uint32_t id = simp::NodeFactory::Instance()->GetID(pkg_name, node_name);
	if (id != 0xffffffff) {
		return Create(id, type);
	} else {
		return NULL;
	}
}

}