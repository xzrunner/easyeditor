#ifndef _COCO_EPE_COCO_PACKER_H_
#define _COCO_EPE_COCO_PACKER_H_


#include <easybuilder.h>

namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }
namespace escale9 { class Symbol; }

namespace libcoco
{
namespace epe
{

class TexturePacker;
class CocoPacker
{
public:
	CocoPacker(const TexturePacker& tex);
	
	void pack(const std::vector<const ee::Symbol*>& symbols);

	void storeToFile(const std::string& filename);

private:
	void resolvePicture(const ee::ImageSprite* sprite);
	void resolvePicture(const ee::ImageSymbol* symbol);

	void resolveFont(const ee::FontBlankSprite* sprite);

	void resolveAnimation(const ecomplex::Symbol* symbol);
	void resolveAnimation(const libanim::Symbol* symbol);
	void resolveAnimation(const escale9::Symbol* symbol);

	void resolveSpriteForComponent(const ee::Sprite* sprite, 
		std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
		std::vector<std::pair<int, std::string> >& order);

	void resolveSpriteForFrame(const ee::Sprite* sprite, int index,
		const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
	void resolveSpriteForFrame(const ee::Sprite* sprite, 
		const std::vector<std::pair<int, std::string> >& order);
	void resolveSpriteForFrame(const ee::Sprite* sprite, int id, bool forceMat);
	void resolveSpriteForFrameImage(const ee::Sprite* sprite, int id);
	void resolveSpriteForFrameFont(const ee::FontBlankSprite* font, int id);

	static void transToMat(const ee::Sprite* sprite, float mat[6], bool force = false);

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