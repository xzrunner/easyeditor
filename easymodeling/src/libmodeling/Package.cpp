#include "Package.h"
#include "Body.h"
#include "Fixture.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "DistanceJoint.h"
#include "PulleyJoint.h"
#include "GearJoint.h"
#include "WheelJoint.h"
#include "WeldJoint.h"
#include "FrictionJoint.h"
#include "RopeJoint.h"
#include "MotorJoint.h"

#include <ee/Math2D.h>

#include <easyshape.h>

namespace emodeling
{

void Paskage::PackBody(const Body& data, std::ofstream& fout)
{
	fout.write(reinterpret_cast<const char*>(&data.m_type), sizeof(int));

	fout.write(reinterpret_cast<const char*>(&data.m_spr->GetPosition().x), sizeof(float));
	fout.write(reinterpret_cast<const char*>(&data.m_spr->GetPosition().y), sizeof(float));
	float angle = data.m_spr->GetAngle();
	fout.write(reinterpret_cast<const char*>(&angle), sizeof(float));

	size_t size = data.m_fixtures.size();
	fout.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (size_t i = 0; i < size; ++i)
	{
		Fixture* fData = data.m_fixtures[i];

		fout.write(reinterpret_cast<const char*>(&fData->m_density), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&fData->m_friction), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&fData->m_restitution), sizeof(float));

		if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(fData->m_shape))
		{
			int type = e_circle;
			fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&circle->radius), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&circle->center.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&circle->center.y), sizeof(float));
		}
		else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(fData->m_shape))
		{
			int type = e_rect;
			fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&rect->m_rect.xmin), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&rect->m_rect.xmax), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&rect->m_rect.ymin), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&rect->m_rect.ymax), sizeof(float));
		}
		else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(fData->m_shape))
		{
			int type = dynamic_cast<eshape::PolygonShape*>(chain) ? e_polygon : e_chain;
			fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

			const std::vector<sm::vec2>& vertices = chain->GetVertices();
			size_t vSize = vertices.size();
			fout.write(reinterpret_cast<const char*>(&vSize), sizeof(size_t));
			for (size_t j = 0; j < vSize; ++j)
			{
				fout.write(reinterpret_cast<const char*>(&vertices[j].x), sizeof(float));
				fout.write(reinterpret_cast<const char*>(&vertices[j].y), sizeof(float));
			}
		}
	}
}

void Paskage::PackJoint(const Joint& data, std::ofstream& fout,
							   const std::vector<Body*>& bodies)
{
	fout.write(reinterpret_cast<const char*>(&data.m_type), sizeof(int));

	switch (data.m_type)
	{
	case Joint::e_revoluteJoint:
		{
			RevoluteJoint* joint = static_cast<RevoluteJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_reference_angle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_enable_limit), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->m_lower_angle), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_upper_angle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_enable_motor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->m_max_motor_torque), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_motor_speed), sizeof(float));
		}
		break;
	case Joint::e_prismaticJoint:
		{
			PrismaticJoint* joint = static_cast<PrismaticJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_axis_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_axis_a.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_reference_angle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_enable_limit), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->m_lower_translation), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_upper_translation), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_enable_motor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->m_max_motor_force), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_motor_speed), sizeof(float));
		}
		break;
	case Joint::e_distanceJoint:
		{
			DistanceJoint* joint = static_cast<DistanceJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.y), sizeof(float));

			float length = ee::Math2D::GetDistance(joint->GetWorldAnchorA(), joint->GetWorldAnchorB());
			fout.write(reinterpret_cast<const char*>(&length), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_frequency_hz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_damping_ratio), sizeof(float));
		}
		break;
	case Joint::e_pulleyJoint:
		{
			PulleyJoint* joint = static_cast<PulleyJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_ground_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_ground_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_ground_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_ground_anchor_b.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_ratio), sizeof(float));
		}
		break;
	case Joint::e_gearJoint:
		{
			GearJoint* joint = static_cast<GearJoint*>(const_cast<Joint*>(&data));

			// todo: 

			fout.write(reinterpret_cast<const char*>(&joint->m_ratio), sizeof(float));
		}
		break;
	case Joint::e_wheelJoint:
		{
			WheelJoint* joint = static_cast<WheelJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			sm::vec2 anchor = joint->GetWorldAnchorB();
			fout.write(reinterpret_cast<const char*>(&anchor.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&anchor.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_axis_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_axis_a.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_enable_motor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->m_max_motor_torque), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_motor_speed), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_frequency_hz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_damping_ratio), sizeof(float));
		}
		break;
	case Joint::e_weldJoint:
		{
			WeldJoint* joint = static_cast<WeldJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_reference_angle), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_frequency_hz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_damping_ratio), sizeof(float));
		}
		break;
	case Joint::e_frictionJoint:
		{
			FrictionJoint* joint = static_cast<FrictionJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_max_force), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_max_torque), sizeof(float));
		}
		break;
	case Joint::e_ropeJoint:
		{
			RopeJoint* joint = static_cast<RopeJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_a.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_local_anchor_b.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->m_max_length), sizeof(float));
		}
		break;
	case Joint::e_motorJoint:
		{
			MotorJoint* joint = static_cast<MotorJoint*>(const_cast<Joint*>(&data));

			int bodyA = QueryBodyIndex(joint->m_body_a, bodies);
			int bodyB = QueryBodyIndex(joint->m_body_b, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_collide_connected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->m_max_force), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_max_torque), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->m_correction_factor), sizeof(float));
		}
		break;
	}
}

int Paskage::QueryBodyIndex(const Body* body, const std::vector<Body*>& bodies)
{
	int ret = -1;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		if (body == bodies[i])
		{
			ret = i;
			break;
		}
	}
	return ret;
}

}