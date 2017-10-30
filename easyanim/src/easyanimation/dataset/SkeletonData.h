#ifndef _EASYANIM_SKELETON_DATA_H_
#define _EASYANIM_SKELETON_DATA_H_

#include <ee/Sprite.h>

#include <SM_Vector.h>

#include <vector>
#include <map>

namespace eanim
{

class Joint;

class SkeletonData
{
public:
	~SkeletonData();

	void CopyFrom(const SkeletonData& skeleton);
	void CopyFrom(const std::vector<ee::SprPtr>& sprs,
		const SkeletonData& skeleton);

	void RemoveSprite(const ee::SprPtr& spr);
	bool IsContainSprite(const ee::SprPtr& spr) const;

	void InsertJoint(const ee::SprPtr& spr, const sm::vec2& pos);
	void RemoveJoint(sm::vec2& pos);

	Joint* QueryJointByPos(const sm::vec2& pos);

	void Draw() const;

	void Absorb(const ee::SprPtr& spr);
	void FixJoint(const ee::SprPtr& spr);
	void UpdateJoint(const ee::SprPtr& spr, float dAngle = 0);

	bool Empty() const { return m_map_joints.empty(); }

	void InsertJoints(const ee::SprPtr& spr, const std::vector<Joint*>& joints) {
		m_map_joints.insert(std::make_pair(spr, joints));
	}

	auto& GetMapJoints() const { return m_map_joints; }

	static void GetTweenSprites(SkeletonData& start, SkeletonData& end, 
		std::vector<ee::SprPtr>& tween, int time, int tot_time);

private:
	void Clean();

	void Translate(const ee::SprPtr& spr, const sm::vec2& offset);

	static ee::SprPtr GetSpriteByName(const std::vector<ee::SprPtr>& sprs, int name);

private:
	std::map<ee::SprPtr, std::vector<Joint*> > m_map_joints;

}; // SkeletonData

}

#endif // _EASYANIM_SKELETON_DATA_H_