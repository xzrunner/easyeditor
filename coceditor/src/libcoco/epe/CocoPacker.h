#ifndef _COCO_EPE_COCO_PACKER_H_
#define _COCO_EPE_COCO_PACKER_H_

#include <easybuilder.h>

#include <map>

namespace ecomplex { class Symbol; }
namespace eanim { class Symbol; }
namespace escale9 { class Symbol; }

namespace ee { class Symbol; class ImageSprite; class ImageSymbol; class FontBlankSprite; class Sprite; }

namespace ecoco
{
namespace epe
{

class TexturePacker;
class CocoPacker
{
public:
	CocoPacker(const TexturePacker& tex);
	
	void pack(const std::vector<const ee::Symbol*>& syms);

	void storeToFile(const std::string& filename);

private:
	void resolvePicture(const ee::ImageSprite* spr);
	void resolvePicture(const ee::ImageSymbol* sym);

	void resolveFont(const ee::FontBlankSprite* spr);

	void resolveAnimation(const ecomplex::Symbol* sym);
	void resolveAnimation(const eanim::Symbol* sym);
	void resolveAnimation(const escale9::Symbol* sym);

	void resolveSpriteForComponent(const ee::Sprite* spr, 
		std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
		std::vector<std::pair<int, std::string> >& order);

	void resolveSpriteForFrame(const ee::Sprite* spr, int index,
		const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
	void resolveSpriteForFrame(const ee::Sprite* spr, 
		const std::vector<std::pair<int, std::string> >& order);
	void resolveSpriteForFrame(const ee::Sprite* spr, int id, bool forceMat);
	void resolveSpriteForFrameImage(const ee::Sprite* spr, int id);
	void resolveSpriteForFrameFont(const ee::FontBlankSprite* font, int id);

	static void transToMat(const ee::Sprite* spr, float mat[6], bool force = false);

private:
	ebuilder::CodeGenerator m_gen;
	const TexturePacker& m_tex;

	int m_id;
	std::map<const ee::Sprite*, int> m_mapSpriteID;
	std::map<const ee::Symbol*, int> m_mapSymbolID;

}; // CocoPacker

}
}

#endif // _COCO_EPE_COCO_PACKER_H_