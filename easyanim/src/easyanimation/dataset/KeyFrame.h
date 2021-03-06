#ifndef _EASYANIM_KEY_FRAME_H_
#define _EASYANIM_KEY_FRAME_H_

#include "SkeletonData.h"

#include <ee/Sprite.h>

#include <sprite2/AnimLerp.h>
#include <cu/CU_RefCountObj.h>

namespace s2 { class ILerp; }

namespace eanim
{

class Layer;

class KeyFrame : public cu::RefCountObj
{
public:
	KeyFrame(int time);
	~KeyFrame();

	void SetLayer(Layer* layer);

	void CopyFromOther(const KeyFrame* src);

	bool IsEmpty() const {
		return m_sprs.empty();
	}

	void Insert(const ee::SprPtr& spr, int idx);
	bool Remove(const ee::SprPtr& spr);
	bool Reorder(const ee::SprConstPtr& spr, bool up);
	bool ReorderMost(const ee::SprConstPtr& spr, bool up);
	bool Sort(std::vector<ee::SprPtr>& sprs);

	int Size() const { return m_sprs.size(); }
	auto GetSprite(int index) {
		return index >= 0 && index < m_sprs.size() ? m_sprs[index] : nullptr;
	}

	const std::vector<ee::SprPtr>& GetAllSprites() const { return m_sprs; }

	bool HasClassicTween() const { return m_classic_tween; }
	void SetClassicTween(bool tween = true) { m_classic_tween = tween; }

	int GetTime() const { return m_time; }
	void SetTime(int time) { m_time = time; }

	void Clear();

	SkeletonData& GetSkeletonData() { return m_skeleton; }

	int GetID() const { return m_id; }
	void SetID(int id) { m_id = id; }

	void OnActive();

	void SetLerp(s2::AnimLerp::SprData data, std::unique_ptr<s2::ILerp> lerp);
	const CU_VEC<std::pair<s2::AnimLerp::SprData, std::unique_ptr<s2::ILerp>>>& GetLerps() const { return m_lerps; }

	static void GetTweenSprite(const KeyFrame* start, const KeyFrame* end, 
		std::vector<ee::SprPtr>& tween, int time, int tot_time);

// private:
// 	void GetTweenSprite(const ee::SprPtr& start, const ee::SprPtr& end, const ee::SprPtr& tween, float process) const;
// 	bool IsTweenMatched(const ee::SprPtr s0, const ee::SprPtr s1) const;

private:
	int m_time;

	std::vector<ee::SprPtr> m_sprs;

	bool m_classic_tween;
	CU_VEC<std::pair<s2::AnimLerp::SprData, std::unique_ptr<s2::ILerp>>> m_lerps;

	Layer* m_layer;

	SkeletonData m_skeleton;

	int m_id;

}; // KeyFrame

}

#endif // _EASYANIM_KEY_FRAME_H_
