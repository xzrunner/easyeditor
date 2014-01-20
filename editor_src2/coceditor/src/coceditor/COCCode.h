#pragma once

#include <easybuilder.h>

namespace coceditor
{
	class COCParser;

	class COCCode
	{
	public:
		COCCode(ebuilder::CodeGenerator& gen);

		void resolve();

	private:
		enum PicFixType
		{
			e_null = 0,
			e_xfix,
			e_yfix,
			e_bothfix,
		};

	private:
		void resolveFromParser(const COCParser& parser);

		void resolvePicture(const d2d::ImageSprite* sprite, const COCParser& parser, 
			PicFixType tsrc = e_null, PicFixType tscreen = e_null);
		void resolvePicture(const d2d::ImageSymbol* symbol, const COCParser& parser,
			PicFixType tsrc = e_null);
		void resolveAnimation(const d2d::ComplexSymbol* symbol);
		//void resolveAnimation(const d2d::AnimSymbol* symbol);
		void resolveAnimation(const d2d::AnimSymbol* symbol);
		void resolveAnimation(const d2d::Patch9Symbol* symbol);

		void resolveAnimationCommon(const d2d::ISymbol* symbol);

		void resolveSpriteForComponent(const d2d::ISprite* sprite, 
			std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
			std::vector<std::pair<int, std::string> >& order);
		void resolveSpriteForFrame(const d2d::ISprite* sprite, int index,
			const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
		void resolveSpriteForFrame(const d2d::ISprite* sprite, 
			const std::vector<std::pair<int, std::string> >& order);
		void resolveSpriteForFrame(const d2d::ISprite* sprite, int id, bool forceMat);
		void resolveSpriteForFrameImage(const d2d::ISprite* sprite, int id);
		void resolveSpriteForFrameFont(const d2d::FontSprite* font, int id);

		static void transToMat(const d2d::ISprite* sprite, float mat[6], bool force = false);

	private:
		ebuilder::CodeGenerator& m_gen;

		int m_id;
		std::map<const d2d::ISprite*, int> m_mapSpriteID;
		std::map<const d2d::ISymbol*, int> m_mapSymbolID;

	}; // COCCode
}

