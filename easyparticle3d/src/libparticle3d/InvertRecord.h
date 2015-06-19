#ifndef _EASYPARTICLE3D_INVERT_RECORDER_H_
#define _EASYPARTICLE3D_INVERT_RECORDER_H_

#include <drag2d.h>

namespace eparticle3d
{

struct Particle;

class InvertRecord
{
public:
	~InvertRecord();

	void AddItem(Particle* p);

	void StoreToFile(const std::string& filepath) const;

	void Clear();

private:
	struct Item
	{
		Item(Particle* p);

		void StoreToFile(Json::Value& val) const;

		clock_t dead_time;

		float life;
		float direction[2];
		float position[3];
		float speed[3];
		float linear_acc;
		float rotate;
		float angle;

		std::string symbol;
		float start_scale, end_scale; 

	}; // Item

private:
	std::vector<Item*> m_items;

}; // InvertRecord

}

#endif // _EASYPARTICLE3D_INVERT_RECORDER_H_