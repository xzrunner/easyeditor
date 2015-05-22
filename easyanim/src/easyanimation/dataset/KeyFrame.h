#ifndef _EASYANIM_KEY_FRAME_H_
#define _EASYANIM_KEY_FRAME_H_

#include <drag2d.h>

#include "SkeletonData.h"

namespace eanim
{

class Controller;
class Layer;

class KeyFrame : public d2d::Object
{
public:
	KeyFrame(Controller* ctrl, int time);
	~KeyFrame();

	void SetLayer(Layer* layer) {
		m_layer = layer;
	}

	void CopyFromOther(const KeyFrame* src);

	bool IsEmpty() const {
		return m_sprites.empty();
	}

	void Insert(d2d::ISprite* sprite);
	bool Remove(d2d::ISprite* sprite);
	void Reorder(const d2d::ISprite* sprite, bool up);

	int Size() const { return m_sprites.size(); }
	const d2d::ISprite* GetSprite(int index) {
		return index >= 0 && index < m_sprites.size() ? m_sprites[index] : NULL;
	}

	const std::vector<d2d::ISprite*>& GetAllSprites() const { return m_sprites; }

	bool HasClassicTween() const { return m_classic_tween; }
	void SetClassicTween(bool tween = true) { m_classic_tween = tween; }

	int GetTime() const { return m_time; }
	void SetTime(int time) { m_time = time; }

	void Clear();

	SkeletonData& GetSkeletonData() { return m_skeletonData; }

	void GetTweenSprite(const KeyFrame* start, const KeyFrame* end, std::vector<d2d::ISprite*>& tween, float process) const;

	int GetID() const { return m_id; }
	void SetID(int id) { m_id = id; }

private:
	void GetTweenSprite(d2d::ISprite* start, d2d::ISprite* end, d2d::ISprite* tween, float process) const;

	bool IsTweenMatched(const d2d::ISprite* s0, const d2d::ISprite* s1) const;

private:
	Controller* m_ctrl;

	int m_layer_idx, m_frame_idx;

	int m_time;

	std::vector<d2d::ISprite*> m_sprites;

	bool m_classic_tween;

	Layer* m_layer;

	SkeletonData m_skeletonData;

	int m_id;

}; // KeyFrame

}

#endif // _EASYANIM_KEY_FRAME_H_
