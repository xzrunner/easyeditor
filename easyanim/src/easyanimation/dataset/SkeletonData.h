#ifndef _EASYANIM_SKELETON_DATA_H_
#define _EASYANIM_SKELETON_DATA_H_

#include <drag2d.h>

namespace eanim
{

class Joint;

class SkeletonData
{
public:
	~SkeletonData();

	void CopyFrom(const SkeletonData& skeleton);
	void CopyFrom(const std::vector<d2d::ISprite*>& sprites,
		const SkeletonData& skeleton);

	void RemoveSprite(d2d::ISprite* sprite);
	bool IsContainSprite(d2d::ISprite* sprite) const;

	void InsertJoint(d2d::ISprite* sprite, const d2d::Vector& pos);
	void RemoveJoint(d2d::Vector& pos);

	Joint* QueryJointByPos(const d2d::Vector& pos);

	void Draw() const;

	void Absorb(d2d::ISprite* sprite);
	void FixJoint(d2d::ISprite* sprite);
	void UpdateJoint(d2d::ISprite* sprite, float dAngle = 0);

	static void GetTweenSprites(SkeletonData& start, SkeletonData& end, 
		std::vector<d2d::ISprite*>& tween, float process);

private:
	void Clean();

	void Translate(d2d::ISprite* sprite, const d2d::Vector& offset);

	static d2d::ISprite* GetSpriteByName(const std::vector<d2d::ISprite*>& sprites, const std::string& name);

private:
	std::map<d2d::ISprite*, std::vector<Joint*> > m_map_joints;

	friend class FileIO;

}; // SkeletonData

}

#endif // _EASYANIM_SKELETON_DATA_H_