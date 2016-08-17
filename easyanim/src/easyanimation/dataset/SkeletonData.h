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
	void CopyFrom(const std::vector<ee::Sprite*>& sprs,
		const SkeletonData& skeleton);

	void RemoveSprite(ee::Sprite* spr);
	bool IsContainSprite(ee::Sprite* spr) const;

	void InsertJoint(ee::Sprite* spr, const sm::vec2& pos);
	void RemoveJoint(sm::vec2& pos);

	Joint* QueryJointByPos(const sm::vec2& pos);

	void Draw() const;

	void Absorb(ee::Sprite* spr);
	void FixJoint(ee::Sprite* spr);
	void UpdateJoint(ee::Sprite* spr, float dAngle = 0);

	bool Empty() const { return m_map_joints.empty(); }

	static void GetTweenSprites(SkeletonData& start, SkeletonData& end, 
		std::vector<ee::Sprite*>& tween, float process);

private:
	void Clean();

	void Translate(ee::Sprite* spr, const sm::vec2& offset);

	static ee::Sprite* GetSpriteByName(const std::vector<ee::Sprite*>& sprs, const std::string& name);

private:
	std::map<ee::Sprite*, std::vector<Joint*> > m_map_joints;

	friend class FileIO;

}; // SkeletonData

}

#endif // _EASYANIM_SKELETON_DATA_H_