#pragma once

#include <drag2d.h>
#include <easybuilder.h>

namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }
namespace escale9 { class Symbol; }

namespace libcoco
{

class TexturePacker;
class CocoPackerNew
{
public:
	CocoPackerNew(const TexturePacker& tex);
	
	void pack(const std::vector<const d2d::ISymbol*>& symbols);

	void storeToFile(const std::string& filename);

private:
	void resolvePicture(const d2d::ImageSprite* sprite);
	void resolvePicture(const d2d::ImageSymbol* symbol);

	void resolveFont(const d2d::FontSprite* sprite);

	void resolveAnimation(const ecomplex::Symbol* symbol);
	void resolveAnimation(const libanim::Symbol* symbol);
	void resolveAnimation(const escale9::Symbol* symbol);

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
	ebuilder::CodeGenerator m_gen;
	const TexturePacker& m_tex;

	int m_id;
	std::map<const d2d::ISprite*, int> m_mapSpriteID;
	std::map<const d2d::ISymbol*, int> m_mapSymbolID;

}; // CocoPackerNew

}