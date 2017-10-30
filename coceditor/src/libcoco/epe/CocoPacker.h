#ifndef _COCO_EPE_COCO_PACKER_H_
#define _COCO_EPE_COCO_PACKER_H_

#include <ee/Symbol.h>
#include <ee/Sprite.h>

#include <easybuilder.h>

#include <map>

namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }
namespace escale9 { class Symbol; }

namespace ee { class ImageSprite; class ImageSymbol; class FontBlankSprite; class Sprite; }

namespace ecoco
{
namespace epe
{

class TexturePacker;
class CocoPacker
{
public:
	CocoPacker(const TexturePacker& tex);
	
	void pack(const std::vector<ee::SymConstPtr>& syms);

	void storeToFile(const std::string& filename);

private:
	void resolvePicture(const std::shared_ptr<const ee::ImageSprite>& spr);
	void resolvePicture(const std::shared_ptr<ee::ImageSymbol>& sym);

	void resolveFont(const std::shared_ptr<ee::FontBlankSprite>& spr);

	void resolveAnimation(const std::shared_ptr<const ecomplex::Symbol>& sym);
	void resolveAnimation(const std::shared_ptr<const libanim::Symbol>& sym);
	void resolveAnimation(const std::shared_ptr<const escale9::Symbol>& sym);

	void resolveSpriteForComponent(const ee::SprConstPtr& spr, 
		std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
		std::vector<std::pair<int, std::string> >& order);

	void resolveSpriteForFrame(const ee::SprConstPtr& spr, int index,
		const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
	void resolveSpriteForFrame(const ee::SprConstPtr& spr, 
		const std::vector<std::pair<int, std::string> >& order);
	void resolveSpriteForFrame(const ee::SprConstPtr& spr, int id, bool forceMat);
	void resolveSpriteForFrameImage(const ee::SprConstPtr& spr, int id);
	void resolveSpriteForFrameFont(const std::shared_ptr<const ee::FontBlankSprite>& font, int id);

	static void transToMat(const ee::SprConstPtr& spr, float mat[6], bool force = false);

private:
	ebuilder::CodeGenerator m_gen;
	const TexturePacker& m_tex;

	int m_id;
	std::map<const ee::SprConstPtr, int> m_mapSpriteID;
	std::map<const ee::SymConstPtr, int> m_mapSymbolID;

}; // CocoPacker

}
}

#endif // _COCO_EPE_COCO_PACKER_H_