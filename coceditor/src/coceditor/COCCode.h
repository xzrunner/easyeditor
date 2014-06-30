#ifndef _COCPACKAGE_COCCODE_H_
#define _COCPACKAGE_COCCODE_H_

#include <drag2d.h>

#include "COCParser.h"

namespace ebuilder { class CodeGenerator; }
namespace ecomplex { class Symbol; }
namespace anim { class Symbol; }
namespace escale9 { class Symbol; }
namespace emesh { class Symbol; class Sprite; }

namespace coceditor
{

	class COCCode
	{
	public:
		COCCode(ebuilder::CodeGenerator& gen, float scale = 1.0f);

		void Parser();

	private:
		enum PicFixType
		{
			e_null = 0,
			e_xfix,
			e_yfix,
			e_bothfix,
		};

	private:
		void ResolveSymbols(const COCParser& parser);

		void ParserPicture(const d2d::ImageSprite* sprite, const COCParser& parser, 
			PicFixType tsrc = e_null, PicFixType tscreen = e_null);
		void ParserPicture(const d2d::ImageSymbol* symbol, const COCParser& parser,
			PicFixType tsrc = e_null);
		void ParserComplex(const ecomplex::Symbol* symbol);
		void ParserAnimation(const anim::Symbol* symbol);
		void ParserScale9(const escale9::Symbol* symbol);
		int ParserMesh(const emesh::Sprite* sprite, const COCParser& parser);

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

	private:
		ebuilder::CodeGenerator& m_gen;

		float m_scale;

		COCParser m_parser;

		int m_id;
		std::map<const d2d::ISprite*, int> m_mapSpriteID;
		std::map<const d2d::ISymbol*, int> m_mapSymbolID;

	}; // COCCode

}

#endif // _COCPACKAGE_COCCODE_H_