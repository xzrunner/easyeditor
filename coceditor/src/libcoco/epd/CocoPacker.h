#ifndef _ECOCO_EPD_COCO_PACKER_H_
#define _ECOCO_EPD_COCO_PACKER_H_

#include "TPParser.h"

namespace ebuilder { class CodeGenerator; }
namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }
namespace escale9 { class Symbol; }
namespace emesh { class Symbol; class Sprite; }
namespace eterrain2d { class Symbol; class Sprite; }
namespace etexture { class Symbol; class Sprite; }
namespace eicon { class Symbol; class Sprite; }
namespace epbin { class ImageIDer; }
namespace ee { class ImageSprite; class ImageSymbol; class FontBlankSprite; }

namespace ecoco
{
namespace epd
{

class CocoPacker
{
public:
	CocoPacker(const std::vector<const ee::Symbol*>& syms, 
		const TextureMgr& tex_mgr);
	CocoPacker(const std::vector<const ee::Symbol*>& syms, 
		const TextureMgr& tex_mgr, const std::string& img_id_file, 
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

	void ParserPicture(const ee::ImageSprite* spr, PicFixType tsrc = e_null, PicFixType tscreen = e_null);
	void ParserPicture(const ee::ImageSymbol* sym, PicFixType tsrc = e_null);
	int ParserIcon(const eicon::Sprite* spr);
	void ParserIcon(const eicon::Symbol* sym, float process, int id);
	void ParserComplex(const ecomplex::Symbol* sym);
	void ParserAnimation(const libanim::Symbol* sym);
	void ParserScale9(const escale9::Symbol* sym);

	void CalSrcFromUV(sm::vec2 src[4], TPParser::Picture* picture);
	void CalSrcFromUVFixed(sm::vec2 src[4], TPParser::Picture* picture);
	int ParserMesh(const emesh::Sprite* spr);
	int ParserTerrain2D(const eterrain2d::Sprite* spr);
	int ParserTexture(const etexture::Sprite* spr);

	void ParserSymbolBase(const ee::Symbol* sym);

	void ParserSpriteForComponent(const ee::Sprite* spr, 
		std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
		std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const ee::Sprite* spr, int index,
		const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const ee::Sprite* spr, const std::vector<std::pair<int, std::string> >& order,
		const std::map<int, int>& map_id2idx);
	void ParserSpriteForFrame(const ee::Sprite* spr, int id, bool forceMat);
	void ParserImageForFrame(const ee::Sprite* spr, int id);
	void ParserFontForFrame(const ee::FontBlankSprite* font, int id);

	void TransToMat(const ee::Sprite* spr, float mat[6], bool force = false) const;

	void GetColorAssignParams(const ee::Sprite* spr, std::vector<std::string>& params) const;

	int QueryIconID(const eicon::Sprite* icon) const;

private:
	ebuilder::CodeGenerator* m_gen;

	float m_scale;

	TPParser m_parser;

	int m_id;
	std::map<const ee::Sprite*, int> m_mapSpriteID;
	std::map<const ee::Symbol*, int> m_mapSymbolID;

	// patch for multi mesh symbol
	struct SpriteID
	{
		ee::Sprite* spr;
		int id;
	};
	std::map<const ee::Symbol*, std::vector<SpriteID> > m_map_symbol2ids;

	std::map<const eicon::Symbol*, std::map<float, int> > m_map_icon2ids;

	epbin::ImageIDer* m_img_ider;

	std::set<int> m_terrain2d_ids;

}; // COCCode

}
}

#endif // _ECOCO_EPD_COCO_PACKER_H_