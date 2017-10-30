#ifndef _ECOCO_EPD_COCO_PACKER_H_
#define _ECOCO_EPD_COCO_PACKER_H_

#include "TPParser.h"

#include <ee/Sprite.h>

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
	CocoPacker(const std::vector<ee::SymPtr>& syms, 
		const TextureMgr& tex_mgr);
	CocoPacker(const std::vector<ee::SymPtr>& syms, 
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

	void ParserPicture(const std::shared_ptr<const ee::ImageSprite>& spr, PicFixType tsrc = e_null, PicFixType tscreen = e_null);
	void ParserPicture(const std::shared_ptr<const ee::ImageSymbol>& sym, PicFixType tsrc = e_null);
	int ParserIcon(const std::shared_ptr<eicon::Sprite>& spr);
	void ParserIcon(const std::shared_ptr<eicon::Symbol>& sym, float process, int id);
	void ParserComplex(const std::shared_ptr<ecomplex::Symbol>& sym);
	void ParserAnimation(const std::shared_ptr<const libanim::Symbol>& sym);
	void ParserScale9(const std::shared_ptr<const escale9::Symbol>& sym);

	void CalSrcFromUV(sm::vec2 src[4], TPParser::Picture* picture);
	void CalSrcFromUVFixed(sm::vec2 src[4], TPParser::Picture* picture);
	int ParserMesh(const std::shared_ptr<emesh::Sprite>& spr);
	int ParserTerrain2D(const std::shared_ptr<eterrain2d::Sprite>& spr);
	int ParserTexture(const std::shared_ptr<etexture::Sprite>& spr);

	void ParserSymbolBase(const ee::SymConstPtr& sym);

	void ParserSpriteForComponent(const ee::SprConstPtr& spr, 
		std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
		std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const ee::SprConstPtr& spr, int index,
		const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const ee::SprConstPtr& spr, const std::vector<std::pair<int, std::string> >& order,
		const std::map<int, int>& map_id2idx);
	void ParserSpriteForFrame(const ee::SprConstPtr& spr, int id, bool forceMat);
	void ParserImageForFrame(const ee::SprConstPtr& spr, int id);
	void ParserFontForFrame(const std::shared_ptr<const ee::FontBlankSprite>& font, int id);

	void TransToMat(const ee::SprConstPtr& spr, float mat[6], bool force = false) const;

	void GetColorAssignParams(const ee::SprConstPtr& spr, std::vector<std::string>& params) const;

	int QueryIconID(const std::shared_ptr<const eicon::Sprite>& icon) const;

private:
	ebuilder::CodeGenerator* m_gen;

	float m_scale;

	TPParser m_parser;

	int m_id;
	std::map<ee::SprConstPtr, int> m_mapSpriteID;
	std::map<ee::SymConstPtr, int> m_mapSymbolID;

	// patch for multi mesh symbol
	struct SpriteID
	{
		ee::SprPtr spr;
		int id;
	};
	std::map<ee::SymPtr, std::vector<SpriteID> > m_map_symbol2ids;

	std::map<std::shared_ptr<const eicon::Symbol>, std::map<float, int> > m_map_icon2ids;

	epbin::ImageIDer* m_img_ider;

	std::set<int> m_terrain2d_ids;

}; // COCCode

}
}

#endif // _ECOCO_EPD_COCO_PACKER_H_