#ifndef _LIBCOCO_COCO_PACKER_H_
#define _LIBCOCO_COCO_PACKER_H_

//#define USE_PACKED_RRP

#include <drag2d.h>

#include "TPParser.h"

namespace ebuilder { class CodeGenerator; }
namespace ecomplex { class Symbol; }
namespace anim { class Symbol; }
namespace escale9 { class Symbol; }
namespace emesh { class Symbol; class Sprite; }

namespace libcoco
{

class CocoPacker
{
public:
	CocoPacker(const std::vector<const d2d::ISymbol*>& symbols, 
		const TextureMgr& tex_mgr);
	CocoPacker(const std::vector<const d2d::ISymbol*>& symbols, 
		const TextureMgr& tex_mgr, const std::string& src_path, 
		float scale = 1.0f);
	~CocoPacker();

	void Parser();

	void Output(const char* filepath) const;

private:
	enum PicFixType
	{
		e_null = 0,
		e_xfix,
		e_yfix,
		e_bothfix,
	};

private:
	void ResolveSymbols();

	void ParserPicture(const d2d::ImageSprite* sprite, PicFixType tsrc = e_null, PicFixType tscreen = e_null);
	void ParserPicture(const d2d::ImageSymbol* symbol, PicFixType tsrc = e_null);
	void ParserComplex(const ecomplex::Symbol* symbol);
	void ParserAnimation(const anim::Symbol* symbol);
	void ParserScale9(const escale9::Symbol* symbol);
	int ParserMesh(const emesh::Sprite* sprite);

	void ParserSymbolBase(const d2d::ISymbol* symbol);

	void ParserSpriteForComponent(const d2d::ISprite* sprite, 
		std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
		std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const d2d::ISprite* sprite, int index,
		const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const d2d::ISprite* sprite, 
		const std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const d2d::ISprite* sprite, int id, bool forceMat);
	void ParserImageForFrame(const d2d::ISprite* sprite, int id);
	void ParserFontForFrame(const d2d::FontSprite* font, int id);

	void TransToMat(const d2d::ISprite* sprite, float mat[6], bool force = false) const;

#ifdef USE_PACKED_RRP
	void LoadImageMapFile(const std::string& filepath);
	int FindImageID(const wxString& filepath) const;
#endif

private:
	ebuilder::CodeGenerator* m_gen;

	const std::string m_src_path;

	float m_scale;

	TPParser m_parser;

	int m_id;
	std::map<const d2d::ISprite*, int> m_mapSpriteID;
	std::map<const d2d::ISymbol*, int> m_mapSymbolID;

	// patch for multi mesh symbol
	struct MeshID
	{
		emesh::Sprite* sprite;
		int id;
	};
	std::map<const emesh::Symbol*, std::vector<MeshID> > m_map_mesh2ids;

#ifdef USE_PACKED_RRP
	std::map<std::string, int> m_rrp_image_id_map;
#endif

}; // COCCode

}

#endif // _LIBCOCO_COCO_PACKER_H_