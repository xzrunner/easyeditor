#include "FileIO.h"
#include "Context.h"

#include "StagePanel.h"

#include <ee/FileHelper.h>
#include <ee/sprite_msg.h>
#include <ee/LibraryPanel.h>
#include <ee/SymbolMgr.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Math2D.h>

#include <easyshape.h>

namespace emodeling
{

void FileIO::Load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Context* context = Context::Instance();

	std::vector<Body*> bodies;

	delete context->world;
	context->world = FileApapter::ToWorld(value["world"]);

	std::string dir = ee::FileHelper::GetFileDir(filename);

	int i = 0;
	Json::Value bodyValue = value["body"][i++];
	while (!bodyValue.isNull()) {
		Body* body = FileApapter::ToBody(bodyValue, dir);
		ee::InsertSpriteSJ::Instance()->Insert(body->m_sprite);
		bodies.push_back(body);

		bodyValue = value["body"][i++];
	}

	i = 0;
	std::vector<Joint*> joints;
	Json::Value jointValue = value["joint"][i++];
	while (!jointValue.isNull()) {
		Joint* joint = FileApapter::ToJoint(jointValue, bodies);
		joints.push_back(joint);
		context->stage->insertJoint(joint);
		jointValue = value["joint"][i++];
	}

	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		if (joints[i]->m_type == Joint::e_gearJoint)
		{
			GearJoint* joint = static_cast<GearJoint*>(joints[i]);
			joint->m_joint1 = joints[value["joint"][i]["joint1"].asInt()];
			joint->m_joint2 = joints[value["joint"][i]["joint2"].asInt()];
		}
	}

	context->library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
}

void FileIO::Store(const char* filename)
{
	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(ee::FetchAllVisitor<Body>(bodies));

	std::vector<Joint*> joints;
	Context::Instance()->stage->traverseJoints(ee::FetchAllVisitor<Joint>(joints));

	Json::Value value;

	std::string dir = ee::FileHelper::GetFileDir(filename);

	value["world"] = B2J(Context::Instance()->world);

	std::map<Body*, int> bodyIndexMap;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		bodyIndexMap[bodies[i]] = i;
		value["body"][i] = B2J(bodies[i], dir);
	}

	std::map<Joint*, int> jointIndexMap;
	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		jointIndexMap[joints[i]] = i;
		value["joint"][i] = B2J(joints[i], bodyIndexMap);
	}

	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		if (joints[i]->m_type == Joint::e_gearJoint)
		{
			GearJoint* joint = static_cast<GearJoint*>(joints[i]);
			value["joint"][i]["joint1"] = jointIndexMap.find(joint->m_joint1)->second;
			value["joint"][i]["joint2"] = jointIndexMap.find(joint->m_joint2)->second;
		}
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

Json::Value FileIO::B2J(const World* world)
{
	Json::Value value;

	value["gravity"]["x"] = world->gravity.x;
	value["gravity"]["y"] = world->gravity.y;

	value["allowSleep"] = world->allowSleep;
	value["warmStarting"] = world->warmStarting;
	value["continuousPhysics"] = world->continuousPhysics;
	value["subStepping"] = world->subStepping;

	value["velocityIterations"] = world->velocityIterations;
	value["positionIterations"] = world->positionIterations;

	return value;
}

Json::Value FileIO::B2J(const Body* body, const std::string& dlg)
{
	Json::Value value;

	value["name"] = body->m_name;

	value["filepath"] = ee::FileHelper::GetRelativePath(dlg, 
		body->m_sprite->GetSymbol().GetFilepath());

	value["type"] = body->m_type;
	switch (body->m_type)
	{
	case Body::e_static:
		value["type"].setComment("//static", Json::commentAfterOnSameLine);
		break;
	case Body::e_dynamic:
		value["type"].setComment("//dynamic", Json::commentAfterOnSameLine);
		break;
	case Body::e_kinematic:
		value["type"].setComment("//kinematic", Json::commentAfterOnSameLine);
		break;
	}

	value["linearDamping"] = body->m_linear_damping;

	value["angularDamping"] = body->m_angular_damping;

	value["allowSleep"] = body->m_allow_sleep;

	value["bullet"] = body->m_bullet;

	value["active"] = body->m_active;

	value["gravityScale"] = body->m_gravity_scale;

	value["position"]["x"] = body->m_sprite->GetPosition().x;
	value["position"]["y"] = body->m_sprite->GetPosition().y;

	value["angle"] = body->m_sprite->GetAngle();

	for (size_t i = 0, n = body->m_fixtures.size(); i < n; ++i)
		value["fixture"][i] = B2J(body->m_fixtures[i]);

	return value;
}

Json::Value FileIO::B2J(const Fixture* fixture)
{
	Json::Value value;

	value["name"] = fixture->m_name;
	value["density"] = fixture->m_density;
	value["friction"] = fixture->m_friction;
	value["restitution"] = fixture->m_restitution;
	value["isSensor"] = fixture->m_is_sensor;
	value["categoryBits"] = fixture->m_category_bits;
	value["maskBits"] = fixture->m_mask_bits;
	value["groupIndex"] = fixture->m_group_index;

	// todo
//	value["shape"] = eshape::FileIO::StoreShape(fixture->shape);

	return value;
}

Json::Value FileIO::B2J(Joint* joint, const std::map<Body*, int>& bodyIndexMap)
{
	Json::Value value;

	value["name"] = joint->m_name;
	
	std::map<Body*, int>::const_iterator itrA = bodyIndexMap.find(joint->m_body_a);
	assert(itrA != bodyIndexMap.end());
	value["body_a"] = itrA->second;

	std::map<Body*, int>::const_iterator itrB = bodyIndexMap.find(joint->m_body_b);
	assert(itrB != bodyIndexMap.end());
	value["body_b"] = itrB->second;

	value["collideConnected"] = joint->m_collide_connected;

	switch (joint->m_type)
	{
	case Joint::e_revoluteJoint:
		{
			value["type"] = "revolute";

			RevoluteJoint* rJoint = static_cast<RevoluteJoint*>(joint);

			value["anchorA"]["x"] = rJoint->m_local_anchor_a.x;
			value["anchorA"]["y"] = rJoint->m_local_anchor_a.y;
			value["anchorB"]["x"] = rJoint->m_local_anchor_b.x;
			value["anchorB"]["y"] = rJoint->m_local_anchor_b.y;

			value["refAngle"] = rJoint->m_reference_angle;

			value["enableLimit"] = rJoint->m_enable_limit;
			value["lowerAngle"] = rJoint->m_lower_angle;
			value["upperAngle"] = rJoint->m_upper_angle;

			value["enableMotor"] = rJoint->m_enable_motor;
			value["maxMotorTorque"] = rJoint->m_max_motor_torque;

			value["motorSpeed"] = rJoint->m_motor_speed;
		}
		break;
	case Joint::e_prismaticJoint:
		{
			value["type"] = "prismatic";

			PrismaticJoint* rJoint = static_cast<PrismaticJoint*>(joint);

			value["anchorA"]["x"] = rJoint->m_local_anchor_a.x;
			value["anchorA"]["y"] = rJoint->m_local_anchor_a.y;
			value["anchorB"]["x"] = rJoint->m_local_anchor_b.x;
			value["anchorB"]["y"] = rJoint->m_local_anchor_b.y;

			value["localAxisA"]["x"] = rJoint->m_local_axis_a.x;
			value["localAxisA"]["y"] = rJoint->m_local_axis_a.y;

			value["refAngle"] = rJoint->m_reference_angle;

			value["enableLimit"] = rJoint->m_enable_limit;
			value["lowerTranslation"] = rJoint->m_lower_translation;
			value["upperTranslation"] = rJoint->m_upper_translation;

			value["enableMotor"] = rJoint->m_enable_motor;
			value["maxMotorForce"] = rJoint->m_max_motor_force;

			value["motorSpeed"] = rJoint->m_motor_speed;
		}
		break;
	case Joint::e_distanceJoint:
		{
			value["type"] = "distance";

			DistanceJoint* dJoint = static_cast<DistanceJoint*>(joint);

			value["anchorA"]["x"] = dJoint->m_local_anchor_a.x;
			value["anchorA"]["y"] = dJoint->m_local_anchor_a.y;
			value["anchorB"]["x"] = dJoint->m_local_anchor_b.x;
			value["anchorB"]["y"] = dJoint->m_local_anchor_b.y;

			value["frequencyHz"] = dJoint->m_frequency_hz;
			value["dampingRatio"] = dJoint->m_damping_ratio;
		}
		break;
	case Joint::e_pulleyJoint:
		{
			value["type"] = "pulley";

			PulleyJoint* pJoint = static_cast<PulleyJoint*>(joint);

			value["ratio"] = pJoint->m_ratio;
		}
		break;
	case Joint::e_gearJoint:
		{
			value["type"] = "gear";

			GearJoint* gJoint = static_cast<GearJoint*>(joint);

			value["ratio"] = gJoint->m_ratio;
		}
		break;
	case Joint::e_wheelJoint:
		{
			value["type"] = "wheel";

			WheelJoint* wJoint = static_cast<WheelJoint*>(joint);

			value["anchorA"]["x"] = wJoint->m_local_anchor_a.x;
			value["anchorA"]["y"] = wJoint->m_local_anchor_a.y;
			value["anchorB"]["x"] = wJoint->m_local_anchor_b.x;
			value["anchorB"]["y"] = wJoint->m_local_anchor_b.y;

			float len = ee::Math2D::GetDistance(wJoint->m_local_axis_a, ee::Vector());
			value["AxisA"]["x"] = wJoint->m_local_axis_a.x / len;
			value["AxisA"]["y"] = wJoint->m_local_axis_a.y / len;

			value["enableMotor"] = wJoint->m_enable_motor;
			value["maxMotorTorque"] = wJoint->m_max_motor_torque;

			value["motorSpeed"] = wJoint->m_motor_speed;

			value["frequencyHz"] = wJoint->m_frequency_hz;
			value["dampingRatio"] = wJoint->m_damping_ratio;
		}
		break;
	case Joint::e_weldJoint:
		{
			value["type"] = "weld";

			WeldJoint* wJoint = static_cast<WeldJoint*>(joint);

			value["anchorA"]["x"] = wJoint->m_local_anchor_a.x;
			value["anchorA"]["y"] = wJoint->m_local_anchor_a.y;
			value["anchorB"]["x"] = wJoint->m_local_anchor_b.x;
			value["anchorB"]["y"] = wJoint->m_local_anchor_b.y;

			value["referenceAngle"] = wJoint->m_damping_ratio;
			value["frequencyHz"] = wJoint->m_frequency_hz;
			value["dampingRatio"] = wJoint->m_damping_ratio;
		}
		break;
	case Joint::e_frictionJoint:
		{
			value["type"] = "friction";

			FrictionJoint* fJoint = static_cast<FrictionJoint*>(joint);

			value["anchorA"]["x"] = fJoint->m_local_anchor_a.x;
			value["anchorA"]["y"] = fJoint->m_local_anchor_a.y;
			value["anchorB"]["x"] = fJoint->m_local_anchor_b.x;
			value["anchorB"]["y"] = fJoint->m_local_anchor_b.y;

			value["maxForce"] = fJoint->m_max_force;
			value["maxTorque"] = fJoint->m_max_torque;
		}
		break;
	case Joint::e_ropeJoint:
		{
			value["type"] = "rope";

			RopeJoint* rJoint = static_cast<RopeJoint*>(joint);

			value["anchorA"]["x"] = rJoint->m_local_anchor_a.x;
			value["anchorA"]["y"] = rJoint->m_local_anchor_a.y;
			value["anchorB"]["x"] = rJoint->m_local_anchor_b.x;
			value["anchorB"]["y"] = rJoint->m_local_anchor_b.y;

			value["maxLength"] = rJoint->m_max_length;
		}
		break;
	case Joint::e_motorJoint:
		{
			value["type"] = "motor";

			MotorJoint* mJoint = static_cast<MotorJoint*>(joint);

			value["maxForce"] = mJoint->m_max_force;
			value["maxTorque"] = mJoint->m_max_torque;
			value["correctionFactor"] = mJoint->m_correction_factor;
		}
		break;
	}

	return value;
}

}