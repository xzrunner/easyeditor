#ifndef _EASYPARTICLE3D_ANIM_RECORDER_H_
#define _EASYPARTICLE3D_ANIM_RECORDER_H_

#include <ee/Color.h>
#include <ee/PointerPool.h>

#include <vector>

namespace eparticle3d
{

class AnimRecorder
{
public:
	AnimRecorder(int max_per_frame);
	~AnimRecorder();

	void AddItem(const std::string& filepath, float x, float y, float angle, 
		float scale, const ee::Colorf& mul_col, const ee::Colorf& add_col);
	void FinishFrame();

	void Clear();

	void StoreToFile(const std::string& filepath) const;

private:
	struct Item
	{
		std::string filepath;
		float x, y;
		float angle;
		float scale;
		ee::Colorf mul_col, add_col;
	};

	struct Frame
	{
		~Frame();

//		void Clear();

		std::vector<Item*> items;
	};

private:
	std::vector<Frame*> m_frames;

	Frame* m_curr_frame;

	ee::PointerPool<Frame> m_frame_pool;
	ee::PointerPool<Item> m_item_pool;

}; // AnimRecorder

}

#endif // _EASYPARTICLE3D_ANIM_RECORDER_H_