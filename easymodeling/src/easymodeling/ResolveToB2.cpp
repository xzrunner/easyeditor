#include "ResolveToB2.h"

#include <ee/physics_const.h>
#include <ee/Math2D.h>

#include <easymodeling.h>
#include <easyshape.h>

namespace emodeling
{

b2Body* ResolveToB2::CreateBody(const Body& data, b2World* world,
								std::map<Body*, b2Body*>& mapBody)
{
	b2BodyDef bd;
	switch (data.m_type)
	{
	case Body::e_static:
		bd.type = b2_staticBody;
		break;
	case Body::e_kinematic:
		bd.type = b2_kinematicBody;
		break;
	case Body::e_dynamic:
		bd.type = b2_dynamicBody;
		break;
	}
	bd.linearDamping = data.m_linear_damping;
	bd.angularDamping = data.m_angular_damping;
	bd.allowSleep = data.m_allow_sleep;
	bd.bullet = data.m_bullet;
	bd.active = data.m_active;
	bd.gravityScale = data.m_gravity_scale;
	b2Body* body = world->CreateBody(&bd);
	mapBody.insert(std::make_pair(const_cast<Body*>(&data), body));

	b2Vec2 pos;
	pos.x = data.m_sprite->GetPosition().x / ee::BOX2D_SCALE_FACTOR;
	pos.y = data.m_sprite->GetPosition().y / ee::BOX2D_SCALE_FACTOR;
	body->SetTransform(pos, data.m_sprite->GetAngle());

	for (size_t i = 0, n = data.m_fixtures.size(); i < n; ++i)
	{
		Fixture* fData = data.m_fixtures[i];

		b2FixtureDef fd;
		fd.density = fData->m_density;
		fd.friction = fData->m_friction;
		fd.restitution = fData->m_restitution;
		fd.isSensor = fData->m_is_sensor;
		fd.filter.categoryBits = fData->m_category_bits;
		fd.filter.maskBits = fData->m_mask_bits;
		fd.filter.groupIndex = fData->m_group_index;

		if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(fData->m_shape))
		{
			b2CircleShape shape;
			shape.m_radius = circle->radius / ee::BOX2D_SCALE_FACTOR;
			shape.m_p.x = circle->center.x / ee::BOX2D_SCALE_FACTOR;
			shape.m_p.y = circle->center.y / ee::BOX2D_SCALE_FACTOR;

			fd.shape = &shape;
			body->CreateFixture(&fd);
		}
		else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(fData->m_shape))
		{
			const float hx = (rect->m_rect.xmax - rect->m_rect.xmin) * 0.5f / ee::BOX2D_SCALE_FACTOR,
				hy = (rect->m_rect.ymax - rect->m_rect.ymin) * 0.5f / ee::BOX2D_SCALE_FACTOR;
			const float cx = (rect->m_rect.xmax + rect->m_rect.xmin) * 0.5f / ee::BOX2D_SCALE_FACTOR,
				cy = (rect->m_rect.ymax + rect->m_rect.ymin) * 0.5f / ee::BOX2D_SCALE_FACTOR;

			b2PolygonShape shape;
			shape.SetAsBox(hx, hy, b2Vec2(cx, cy), 0);

			fd.shape = &shape;
			body->CreateFixture(&fd);
		}
		else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(fData->m_shape))
		{
// 			const std::vector<sm::vec2>& src = polygon->GetVertices();
// 			const size_t size = src.size();
// 			std::vector<b2Vec2> dst(size);
// 			for (size_t j = 0; j < size; ++j)
// 			{
// 				dst[j].x = src[j].x / ee::BOX2D_SCALE_FACTOR;
// 				dst[j].y = src[j].y / ee::BOX2D_SCALE_FACTOR;
// 			}
// 
// 			b2PolygonShape shape;
// 			shape.Set(&dst[0], size);
// 			fd.shape = &shape;
// 
// 			body->CreateFixture(&fd);

			//////////////////////////////////////////////////////////////////////////

			const std::vector<sm::vec2>& src = polygon->GetVertices();
			for (int i = 0, n = src.size(); i < n; i += 3) {
 				b2Vec2 dst[3];
				for (int j = 0; j < 3; ++j) {
					dst[j].x = src[i+j].x / ee::BOX2D_SCALE_FACTOR;
					dst[j].y = src[i+j].y / ee::BOX2D_SCALE_FACTOR;
				}

				b2PolygonShape shape;
				shape.Set(&dst[0], 3);
				fd.shape = &shape;

				body->CreateFixture(&fd);
			}
		}
		else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(fData->m_shape))
		{
			const std::vector<sm::vec2>& src = chain->GetVertices();
			const size_t size = src.size();
			std::vector<b2Vec2> dst(size);
			for (size_t j = 0; j < size; ++j)
			{
				dst[j].x = src[j].x / ee::BOX2D_SCALE_FACTOR;
				dst[j].y = src[j].y / ee::BOX2D_SCALE_FACTOR;
			}

			b2ChainShape shape;
			if (chain->IsClosed())
				shape.CreateLoop(&dst[0], size);
			else
				shape.CreateChain(&dst[0], size);
			fd.shape = &shape;

			body->CreateFixture(&fd);
		}
	}

	return body;
}

b2Joint* ResolveToB2::CreateJoint(const Joint& data, b2World* world,
								  const std::map<Body*, b2Body*>& mapBody)
{
	b2Joint* bJoint = NULL;

	switch(data.m_type)
	{
	case Joint::e_revoluteJoint:
		{
			b2RevoluteJointDef jd;

			RevoluteJoint* joint = static_cast<RevoluteJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* body_a = itrA->second;
			b2Body* m_body_b = itrB->second;
			jd.Initialize(body_a, m_body_b, b2Vec2(0, 0));
			jd.collideConnected = joint->m_collide_connected;
			jd.localAnchorA.Set(joint->m_local_anchor_a.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_a.y / ee::BOX2D_SCALE_FACTOR);
			jd.localAnchorA.Set(joint->m_local_anchor_b.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_b.y / ee::BOX2D_SCALE_FACTOR);
			jd.referenceAngle = joint->m_reference_angle;
			jd.enableLimit = joint->m_enable_limit;
			jd.lowerAngle = joint->m_lower_angle;
			jd.upperAngle = joint->m_upper_angle;
			jd.enableMotor = joint->m_enable_motor;
			jd.maxMotorTorque = joint->m_max_motor_torque;
			jd.motorSpeed = joint->m_motor_speed;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_prismaticJoint:
		{
			b2PrismaticJointDef jd;

			PrismaticJoint* joint = static_cast<PrismaticJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* body_a = itrA->second;
			b2Body* body_b = itrB->second;

			b2Vec2 axis(joint->m_local_axis_a.x, joint->m_local_axis_a.y);
			axis.Normalize();
			jd.Initialize(body_a, body_b, b2Vec2(0, 0), axis);
			jd.collideConnected = joint->m_collide_connected;
			jd.localAnchorA.Set(joint->m_local_anchor_a.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_a.y / ee::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->m_local_anchor_b.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_b.y / ee::BOX2D_SCALE_FACTOR);
			jd.referenceAngle = joint->m_reference_angle;
			jd.enableLimit = joint->m_enable_limit;
			jd.lowerTranslation = joint->m_lower_translation;
			jd.upperTranslation = joint->m_upper_translation;
			jd.enableMotor = joint->m_enable_motor;
			jd.maxMotorForce = joint->m_max_motor_force;
			jd.motorSpeed = joint->m_motor_speed;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_distanceJoint:
		{
			b2DistanceJointDef jd;

			DistanceJoint* joint = static_cast<DistanceJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->m_collide_connected;
			jd.localAnchorA.Set(joint->m_local_anchor_a.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_a.y / ee::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->m_local_anchor_b.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_b.y / ee::BOX2D_SCALE_FACTOR);
			jd.length = ee::Math2D::GetDistance(joint->GetWorldAnchorA(), joint->GetWorldAnchorB()) / ee::BOX2D_SCALE_FACTOR;
			jd.frequencyHz = joint->m_frequency_hz;
			jd.dampingRatio = joint->m_damping_ratio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_pulleyJoint:
		{
			b2PulleyJointDef jd;

			PulleyJoint* joint = static_cast<PulleyJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* body_a = itrA->second;
			b2Body* body_b = itrB->second;

			b2Vec2 groundAnchorA(joint->m_ground_anchor_a.x / ee::BOX2D_SCALE_FACTOR, joint->m_ground_anchor_a.y / ee::BOX2D_SCALE_FACTOR),
				groundAnchorB(joint->m_ground_anchor_b.x / ee::BOX2D_SCALE_FACTOR, joint->m_ground_anchor_b.y / ee::BOX2D_SCALE_FACTOR);
			sm::vec2 anchorA_ = joint->GetWorldAnchorA(),
				anchorB_ = joint->GetWorldAnchorB();
			b2Vec2 anchorA(anchorA_.x / ee::BOX2D_SCALE_FACTOR, anchorA_.y / ee::BOX2D_SCALE_FACTOR),
				anchorB(anchorB_.x / ee::BOX2D_SCALE_FACTOR, anchorB_.y / ee::BOX2D_SCALE_FACTOR);

			jd.Initialize(body_a, body_b, groundAnchorA, groundAnchorB, anchorA, anchorB, joint->m_ratio);
			jd.collideConnected = joint->m_collide_connected;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_gearJoint:
		{
		}
		break;
	case Joint::e_wheelJoint:
		{
			b2WheelJointDef jd;

			WheelJoint* joint = static_cast<WheelJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			b2Body* body_a = itrA->second;
			b2Body* body_b = itrB->second;

			sm::vec2 pos = joint->GetWorldAnchorB() / ee::BOX2D_SCALE_FACTOR;
			b2Vec2 axis(joint->m_local_axis_a.x, joint->m_local_axis_a.y);
			axis.Normalize();
			jd.Initialize(body_a, body_b, b2Vec2(pos.x, pos.y), axis);
			jd.enableMotor = joint->m_enable_motor;
			jd.maxMotorTorque = joint->m_max_motor_torque;
			jd.motorSpeed = joint->m_motor_speed;
			jd.frequencyHz = joint->m_frequency_hz;
			jd.dampingRatio = joint->m_damping_ratio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_weldJoint:
		{
			b2WeldJointDef jd;

			WeldJoint* joint = static_cast<WeldJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->m_collide_connected;
			jd.localAnchorA.Set(joint->m_local_anchor_a.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_a.y / ee::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->m_local_anchor_b.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_b.y / ee::BOX2D_SCALE_FACTOR);
			jd.referenceAngle = joint->m_reference_angle;
			jd.frequencyHz = joint->m_frequency_hz;
			jd.dampingRatio = joint->m_damping_ratio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_frictionJoint:
		{
			b2FrictionJointDef jd;

			FrictionJoint* joint = static_cast<FrictionJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->m_collide_connected;
			jd.localAnchorA.Set(joint->m_local_anchor_a.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_a.y / ee::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->m_local_anchor_b.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_b.y / ee::BOX2D_SCALE_FACTOR);
			jd.maxForce = joint->m_max_force;
			jd.maxTorque = joint->m_max_torque;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_ropeJoint:
		{
			b2RopeJointDef jd;

			RopeJoint* joint = static_cast<RopeJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;
			jd.collideConnected = joint->m_collide_connected;
			jd.localAnchorA.Set(joint->m_local_anchor_a.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_a.y / ee::BOX2D_SCALE_FACTOR);
			jd.localAnchorB.Set(joint->m_local_anchor_b.x / ee::BOX2D_SCALE_FACTOR, joint->m_local_anchor_b.y / ee::BOX2D_SCALE_FACTOR);
			jd.maxLength = joint->m_max_length;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	case Joint::e_motorJoint:
		{
			b2MotorJointDef jd;

			MotorJoint* joint = static_cast<MotorJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.Initialize(itrA->second, itrB->second);
			jd.collideConnected = joint->m_collide_connected;
			jd.maxForce = joint->m_max_force;
			jd.maxTorque = joint->m_max_torque;
			jd.correctionFactor = joint->m_correction_factor;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	default:
		assert(0);
	}

	return bJoint;
}

b2Joint* ResolveToB2::CreateJoint(const Joint& data, b2World* world,
								  const std::map<Body*, b2Body*>& mapBody,
								  const std::map<Joint*, b2Joint*>& mapJoint)
{
	b2Joint* bJoint = NULL;

	switch(data.m_type)
	{
	case Joint::e_gearJoint:
		{
			b2GearJointDef jd;

			GearJoint* joint = static_cast<GearJoint*>(const_cast<Joint*>(&data));

			std::map<Body*, b2Body*>::const_iterator 
				itrA = mapBody.find(joint->m_body_a),
				itrB = mapBody.find(joint->m_body_b);
			assert(itrA != mapBody.end() && itrB != mapBody.end());
			jd.bodyA = itrA->second;
			jd.bodyB = itrB->second;

			std::map<Joint*, b2Joint*>::const_iterator 
				itr1 = mapJoint.find(joint->m_joint1),
				itr2 = mapJoint.find(joint->m_joint2);
			assert(itr1 != mapJoint.end() && itr2 != mapJoint.end());
			jd.joint1 = itr1->second;
			jd.joint2 = itr2->second;

			jd.ratio = joint->m_ratio;

			bJoint = world->CreateJoint(&jd);
		}
		break;
	}

	return bJoint;
}

}