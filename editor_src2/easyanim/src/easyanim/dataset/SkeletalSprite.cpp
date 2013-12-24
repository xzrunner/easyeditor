#include "SkeletalSprite.h"
#include "WholeSkeleton.h"
#include "SpriteFactory.h"
#include "Symbol.h"

#include "frame/Context.h"
#include "view/LibraryPanel.h"

namespace eanim
{

SkeletalSprite::SkeletalSprite()
{
	m_src = m_skeleton = NULL;
}

SkeletalSprite::SkeletalSprite(WholeSkeleton* skeleton)
{
	m_src = skeleton;
	m_skeleton = skeleton->clone();
	initBounding();
}

SkeletalSprite::SkeletalSprite(const SkeletalSprite& sprite)
	: Sprite(sprite)
{
	m_src = sprite.m_src;
	m_skeleton = sprite.m_skeleton->clone();
}

SkeletalSprite::~SkeletalSprite()
{
	delete m_skeleton;
}

SkeletalSprite* SkeletalSprite::clone()
{
	return new SkeletalSprite(*this);
}

void SkeletalSprite::loadFromTextFile(std::ifstream& fin)
{
	fin >> m_pos >> m_angle;

	int index;
	fin >> index;
	assert(index != -1);
	LibraryPanel* library = Context::Instance()->library;
	m_src = library->getSkeleton(index);
	m_skeleton = library->getSkeleton(index)->clone();

	int size;
	fin >> size;
	std::vector<WholeSkeleton::Sprite*> sprites;
	m_skeleton->traverse(WholeSkeleton::FetchAllBonesVisitor(sprites));
	assert(size == sprites.size());
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->loadFromTextFile(fin);
	
	initBounding();
}

void SkeletalSprite::storeToTextFile(std::ofstream& fout) const
{
// 	fout << SpriteFactory::e_skeletal << " " 
// 		<< m_pos << " " << m_angle << " "
// 		<< library.getIndex(m_src) << " ";

	std::vector<WholeSkeleton::Sprite*> sprites;
	m_skeleton->traverse(WholeSkeleton::FetchAllBonesVisitor(sprites));
	fout << sprites.size() << '\n';
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->storeToTextFile(fout);
}

void SkeletalSprite::reloadTexture()
{
	m_skeleton->traverse(ReloadTextureVisitor());
}

void SkeletalSprite::initBounding()
{
	m_bounding = d2d::BVFactory::createBV(d2d::e_obb);
	m_skeleton->getBounding(*m_bounding);
}

///////////////////////////////////////////////////////////////////////////
// class SkeletalSprite::ReloadTextureVisitor
//////////////////////////////////////////////////////////////////////////

void SkeletalSprite::ReloadTextureVisitor::visit(d2d::ICloneable* object, bool& bFetchNext)
{
	WholeSkeleton::Sprite* bone = static_cast<WholeSkeleton::Sprite*>(object);
	bone->getSymbol()->reloadTexture();
	bFetchNext = true;
}

} // eanim