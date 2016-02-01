
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

#include <easyshape.h>

namespace libmodeling
{

void Paskage::packBody(const Body& data, std::ofstream& fout)
{
	fout.write(reinterpret_cast<const char*>(&data.type), sizeof(int));

	fout.write(reinterpret_cast<const char*>(&data.sprite->GetPosition().x), sizeof(float));
	fout.write(reinterpret_cast<const char*>(&data.sprite->GetPosition().y), sizeof(float));
	float angle = data.sprite->GetAngle();
	fout.write(reinterpret_cast<const char*>(&angle), sizeof(float));

	size_t size = data.fixtures.size();
	fout.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	for (size_t i = 0; i < size; ++i)
	{
		Fixture* fData = data.fixtures[i];

		fout.write(reinterpret_cast<const char*>(&fData->density), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&fData->friction), sizeof(float));
		fout.write(reinterpret_cast<const char*>(&fData->restitution), sizeof(float));

		if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(fData->shape))
		{
			int type = e_circle;
			fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&circle->radius), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&circle->center.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&circle->center.y), sizeof(float));
		}
		else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(fData->shape))
		{
			int type = e_rect;
			fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&rect->m_rect.xmin), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&rect->m_rect.xmax), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&rect->m_rect.ymin), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&rect->m_rect.ymax), sizeof(float));
		}
		else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(fData->shape))
		{
			int type = dynamic_cast<eshape::PolygonShape*>(chain) ? e_polygon : e_chain;
			fout.write(reinterpret_cast<const char*>(&type), sizeof(int));

			const std::vector<ee::Vector>& vertices = chain->GetVertices();
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

void Paskage::packJoint(const Joint& data, std::ofstream& fout,
							   const std::vector<Body*>& bodies)
{
	fout.write(reinterpret_cast<const char*>(&data.type), sizeof(int));

	switch (data.type)
	{
	case Joint::e_revoluteJoint:
		{
			RevoluteJoint* joint = static_cast<RevoluteJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->referenceAngle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableLimit), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->lowerAngle), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->upperAngle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableMotor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->maxMotorTorque), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->motorSpeed), sizeof(float));
		}
		break;
	case Joint::e_prismaticJoint:
		{
			PrismaticJoint* joint = static_cast<PrismaticJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->referenceAngle), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableLimit), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->lowerTranslation), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->upperTranslation), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableMotor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->maxMotorForce), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->motorSpeed), sizeof(float));
		}
		break;
	case Joint::e_distanceJoint:
		{
			DistanceJoint* joint = static_cast<DistanceJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			float length = ee::Math2D::GetDistance(joint->getWorldAnchorA(), joint->getWorldAnchorB());
			fout.write(reinterpret_cast<const char*>(&length), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->frequencyHz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->dampingRatio), sizeof(float));
		}
		break;
	case Joint::e_pulleyJoint:
		{
			PulleyJoint* joint = static_cast<PulleyJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->groundAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->groundAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->groundAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->groundAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->ratio), sizeof(float));
		}
		break;
	case Joint::e_gearJoint:
		{
			GearJoint* joint = static_cast<GearJoint*>(const_cast<Joint*>(&data));

			// todo: 

			fout.write(reinterpret_cast<const char*>(&joint->ratio), sizeof(float));
		}
		break;
	case Joint::e_wheelJoint:
		{
			WheelJoint* joint = static_cast<WheelJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			ee::Vector anchor = joint->getWorldAnchorB();
			fout.write(reinterpret_cast<const char*>(&anchor.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&anchor.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAxisA.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->enableMotor), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&joint->maxMotorTorque), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->motorSpeed), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->frequencyHz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->dampingRatio), sizeof(float));
		}
		break;
	case Joint::e_weldJoint:
		{
			WeldJoint* joint = static_cast<WeldJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->referenceAngle), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->frequencyHz), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->dampingRatio), sizeof(float));
		}
		break;
	case Joint::e_frictionJoint:
		{
			FrictionJoint* joint = static_cast<FrictionJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->maxForce), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->maxTorque), sizeof(float));
		}
		break;
	case Joint::e_ropeJoint:
		{
			RopeJoint* joint = static_cast<RopeJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorA.y), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.x), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->localAnchorB.y), sizeof(float));

			fout.write(reinterpret_cast<const char*>(&joint->maxLength), sizeof(float));
		}
		break;
	case Joint::e_motorJoint:
		{
			MotorJoint* joint = static_cast<MotorJoint*>(const_cast<Joint*>(&data));

			int bodyA = queryBodyIndex(joint->bodyA, bodies);
			int bodyB = queryBodyIndex(joint->bodyB, bodies);
			assert(bodyA != -1 && bodyB != -1);
			fout.write(reinterpret_cast<const char*>(&bodyA), sizeof(int));
			fout.write(reinterpret_cast<const char*>(&bodyB), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->collideConnected), sizeof(int));

			fout.write(reinterpret_cast<const char*>(&joint->maxForce), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->maxTorque), sizeof(float));
			fout.write(reinterpret_cast<const char*>(&joint->correctionFactor), sizeof(float));
		}
		break;
	}
}

int Paskage::queryBodyIndex(const Body* body, const std::vector<Body*>& bodies)
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