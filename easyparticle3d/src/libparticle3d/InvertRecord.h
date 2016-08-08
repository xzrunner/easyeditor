#ifndef _EASYPARTICLE3D_INVERT_RECORDER_H_
#define _EASYPARTICLE3D_INVERT_RECORDER_H_

#include <sprite2/Color.h>

#include <json/json.h>

#include <time.h>

struct p3d_particle;

namespace eparticle3d
{

class InvertRecord
{
public:
	InvertRecord();
	~InvertRecord();

	void AddItem(p3d_particle* p);

	void StoreToFile(const std::string& filepath) const;

	void Clear();

	void SetGravity(float gravity) { m_gravity = gravity; }

private:
	struct Item
	{
		Item(p3d_particle* p);

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
		float scale_start, scale_end; 
		s2::Color mul_col_begin, mul_col_end, 
			      add_col_begin, add_col_end;

	}; // Item

private:
	std::vector<Item*> m_items;

	float m_gravity;

}; // InvertRecord

}

#endif // _EASYPARTICLE3D_INVERT_RECORDER_H_