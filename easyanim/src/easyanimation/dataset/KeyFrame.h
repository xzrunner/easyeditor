#ifndef _EASYANIM_KEY_FRAME_H_
#define _EASYANIM_KEY_FRAME_H_

#include "SkeletonData.h"

#include <CU_RefCountObj.h>

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

	void Insert(ee::Sprite* spr, int idx);
	bool Remove(ee::Sprite* spr);
	bool Reorder(const ee::Sprite* spr, bool up);
	bool ReorderMost(const ee::Sprite* spr, bool up);
	bool Sort(std::vector<ee::Sprite*>& sprs);

	int Size() const { return m_sprs.size(); }
	const ee::Sprite* GetSprite(int index) {
		return index >= 0 && index < m_sprs.size() ? m_sprs[index] : NULL;
	}

	const std::vector<ee::Sprite*>& GetAllSprites() const { return m_sprs; }

	bool HasClassicTween() const { return m_classic_tween; }
	void SetClassicTween(bool tween = true) { m_classic_tween = tween; }

	int GetTime() const { return m_time; }
	void SetTime(int time) { m_time = time; }

	void Clear();

	SkeletonData& GetSkeletonData() { return m_skeleton; }

	int GetID() const { return m_id; }
	void SetID(int id) { m_id = id; }

	void OnActive();

	void SetLerp(int data, s2::ILerp* lerp);
	const std::vector<std::pair<int, s2::ILerp*> >& GetLerps() const { return m_lerps; }

	static void GetTweenSprite(const KeyFrame* start, const KeyFrame* end, std::vector<ee::Sprite*>& tween, float process);

// private:
// 	void GetTweenSprite(ee::Sprite* start, ee::Sprite* end, ee::Sprite* tween, float process) const;
// 	bool IsTweenMatched(const ee::Sprite* s0, const ee::Sprite* s1) const;

private:
	int m_time;

	std::vector<ee::Sprite*> m_sprs;

	bool m_classic_tween;
	std::vector<std::pair<int, s2::ILerp*> > m_lerps;

	Layer* m_layer;

	SkeletonData m_skeleton;

	int m_id;

}; // KeyFrame

}

#endif // _EASYANIM_KEY_FRAME_H_
