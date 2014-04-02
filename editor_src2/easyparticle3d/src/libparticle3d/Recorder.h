#ifndef _EASYPARTICLE3D_RECORDER_H_
#define _EASYPARTICLE3D_RECORDER_H_

#include <drag2d.h>

namespace eparticle3d
{

class Recorder
{
public:
	Recorder();
	~Recorder();

	void AddItem(const std::string& filepath, float x, float y, 
		float angle, float scale, const d2d::Colorf& col);
	void FinishFrame();

	void Clear();

	void StoreToAnimFile(const std::string& filepath) const;

private:
	struct Item
	{
		std::string filepath;
		float x, y;
		float angle;
		float scale;
		d2d::Colorf col;
	};

	struct Frame
	{
		~Frame();

		void Clear();

		std::vector<Item*> items;
	};

private:
	std::vector<Frame*> m_frames;

	Frame* m_curr_frame;

}; // Recorder

}

#endif // _EASYPARTICLE3D_RECORDER_H_