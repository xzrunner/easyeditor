#ifndef _EASYPARTICLE3D_ANIM_RECORDER_H_
#define _EASYPARTICLE3D_ANIM_RECORDER_H_

#include <drag2d.h>

namespace eparticle3d
{

class AnimRecorder
{
public:
	AnimRecorder(int max_per_frame);
	~AnimRecorder();

	void AddItem(const std::string& filepath, float x, float y, float angle, 
		float scale, const d2d::Colorf& mul_col, const d2d::Colorf& add_col);
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
		d2d::Colorf mul_col, add_col;
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

	d2d::PointerPool<Frame> m_frame_pool;
	d2d::PointerPool<Item> m_item_pool;

}; // AnimRecorder

}

#endif // _EASYPARTICLE3D_ANIM_RECORDER_H_