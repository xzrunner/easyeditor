#ifndef _EASYANIM_SKELETON_DATA_H_
#define _EASYANIM_SKELETON_DATA_H_

#include <SM_Vector.h>

#include <vector>
#include <map>

namespace ee { class Sprite; }

namespace eanim
{

class Joint;

class SkeletonData
{
public:
	~SkeletonData();

	void CopyFrom(const SkeletonData& skeleton);
	void CopyFrom(const std::vector<ee::Sprite*>& sprites,
		const SkeletonData& skeleton);

	void RemoveSprite(ee::Sprite* sprite);
	bool IsContainSprite(ee::Sprite* sprite) const;

	void InsertJoint(ee::Sprite* sprite, const sm::vec2& pos);
	void RemoveJoint(sm::vec2& pos);

	Joint* QueryJointByPos(const sm::vec2& pos);

	void Draw() const;

	void Absorb(ee::Sprite* sprite);
	void FixJoint(ee::Sprite* sprite);
	void UpdateJoint(ee::Sprite* sprite, float dAngle = 0);

	bool Empty() const { return m_map_joints.empty(); }

	static void GetTweenSprites(SkeletonData& start, SkeletonData& end, 
		std::vector<ee::Sprite*>& tween, float process);

private:
	void Clean();

	void Translate(ee::Sprite* sprite, const sm::vec2& offset);

	static ee::Sprite* GetSpriteByName(const std::vector<ee::Sprite*>& sprites, const std::string& name);

private:
	std::map<ee::Sprite*, std::vector<Joint*> > m_map_joints;

	friend class FileIO;

}; // SkeletonData

}

#endif // _EASYANIM_SKELETON_DATA_H_