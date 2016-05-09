#include "FileAdapter.h"

#include "World.h"
#include "Body.h"
#include "Fixture.h"
#include "Joint.h"

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

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>

#include <easyshape.h>

namespace emodeling
{

World* FileApapter::ToWorld(const Json::Value& worldValue)
{
	World* world = new World;

	world->gravity.x = worldValue["gravity"]["x"].asDouble();
	world->gravity.y = worldValue["gravity"]["y"].asDouble();

	world->allowSleep = worldValue["allowSleep"].asBool();
	world->warmStarting = worldValue["warmStarting"].asBool();
	world->continuousPhysics = worldValue["continuousPhysics"].asBool();
	world->subStepping = worldValue["subStepping"].asBool();

	world->velocityIterations = worldValue["velocityIterations"].asInt();
	world->positionIterations = worldValue["positionIterations"].asInt();

	return world;
}

Body* FileApapter::ToBody(const Json::Value& bodyValue, const std::string& dlg)
{
	std::string filepath = bodyValue["filepath"].asString();
	filepath = ee::FileHelper::GetAbsolutePath(dlg, filepath);

	ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
	symbol->Release();

	sm::vec2 pos;
	pos.x = bodyValue["position"]["x"].asDouble();
	pos.y = bodyValue["position"]["y"].asDouble();
	float angle = bodyValue["angle"].asDouble();

	sprite->SetTransform(pos, angle);

	Body* body = new Body;
	body->m_name = bodyValue["name"].asString();
	body->m_type = Body::Type(bodyValue["type"].asInt());
	body->m_linear_damping = bodyValue["linearDamping"].asDouble();
	body->m_angular_damping = bodyValue["angularDamping"].asDouble();
	body->m_allow_sleep = bodyValue["allowSleep"].asBool();
	body->m_bullet = bodyValue["bullet"].asBool();
	body->m_active = bodyValue["active"].asBool();
	body->m_gravity_scale = bodyValue["gravityScale"].asDouble();
	body->m_sprite = sprite;

	sprite->SetUserData(body);

	int i = 0;
	Json::Value fixtureValue = bodyValue["fixture"][i++];
	while (!fixtureValue.isNull()) {
		Fixture* fixture = ToFixture(fixtureValue);
		fixture->m_body = body;
		body->m_fixtures.push_back(fixture);

		fixtureValue = bodyValue["fixture"][i++];
	}

	return body;
}

Fixture* FileApapter::ToFixture(const Json::Value& fixtureValue)
{
	Fixture* fixture = new Fixture;

	fixture->m_name = fixtureValue["name"].asString();

	fixture->m_density = fixtureValue["density"].asDouble();
	fixture->m_friction = fixtureValue["friction"].asDouble();
	fixture->m_restitution = fixtureValue["restitution"].asDouble();
	fixture->m_is_sensor = fixtureValue["isSensor"].asBool();
	fixture->m_category_bits = fixtureValue["categoryBits"].asInt();
	fixture->m_mask_bits = fixtureValue["maskBits"].asInt();
	fixture->m_group_index = fixtureValue["groupIndex"].asInt();

	// todo
//	fixture->shape = eshape::FileIO::LoadShape(fixtureValue["shape"]);

	return fixture;
}

Joint* FileApapter::ToJoint(const Json::Value& jointValue,
							 const std::vector<Body*>& bodies)
{
	Joint* joint = NULL;

	int bodyIndexA = jointValue["bodyA"].asInt();
	int bodyIndexB = jointValue["bodyB"].asInt();
	assert(bodyIndexA < bodies.size() && bodyIndexB < bodies.size());

	std::string type = jointValue["type"].asString();
	if (type == "revolute")
	{
		RevoluteJoint* rJoint = new RevoluteJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		rJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		rJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		rJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		rJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		rJoint->m_reference_angle = jointValue["refAngle"].asDouble();

		rJoint->m_enable_limit = jointValue["enableLimit"].asBool();
		rJoint->m_lower_angle = jointValue["lowerAngle"].asDouble();
		rJoint->m_upper_angle = jointValue["upperAngle"].asDouble();

		rJoint->m_enable_motor = jointValue["enableMotor"].asBool();
		rJoint->m_max_motor_torque = jointValue["maxMotorTorque"].asDouble();

		rJoint->m_motor_speed = jointValue["motorSpeed"].asDouble();

		joint = rJoint;
	}
	else if (type == "prismatic")
	{
		PrismaticJoint* pJoint = new PrismaticJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		pJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		pJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		pJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		pJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		pJoint->m_local_axis_a.x = jointValue["localAxisA"]["x"].asDouble();
		pJoint->m_local_axis_a.y = jointValue["localAxisA"]["y"].asDouble();

		pJoint->m_reference_angle = jointValue["refAngle"].asDouble();

		pJoint->m_enable_limit = jointValue["enableLimit"].asBool();
		pJoint->m_lower_translation = jointValue["lowerTranslation"].asDouble();
		pJoint->m_upper_translation = jointValue["upperTranslation"].asDouble();

		pJoint->m_enable_motor = jointValue["enableMotor"].asBool();
		pJoint->m_max_motor_force = jointValue["maxMotorForce"].asDouble();

		pJoint->m_motor_speed = jointValue["motorSpeed"].asDouble();

		joint = pJoint;
	}
	else if (type == "distance")
	{
		DistanceJoint* dJoint = new DistanceJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		dJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		dJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		dJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		dJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		dJoint->m_frequency_hz = jointValue["frequencyHz"].asDouble();
		dJoint->m_damping_ratio = jointValue["dampingRatio"].asDouble();

		joint = dJoint;
	}
	else if (type == "pulley")
	{
		PulleyJoint* pJoint = new PulleyJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		pJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		pJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		pJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		pJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		pJoint->m_ground_anchor_a.x = jointValue["groundAnchorA"]["x"].asDouble();
		pJoint->m_ground_anchor_a.y = jointValue["groundAnchorA"]["y"].asDouble();
		pJoint->m_ground_anchor_b.x = jointValue["groundAnchorB"]["x"].asDouble();
		pJoint->m_ground_anchor_b.y = jointValue["groundAnchorB"]["y"].asDouble();

		pJoint->m_ratio = jointValue["ratio"].asDouble();

		joint = pJoint;
	}
	else if (type == "gear")
	{
		GearJoint* gJoint = new GearJoint(bodies[bodyIndexA], bodies[bodyIndexB], NULL, NULL);

		gJoint->m_ratio = jointValue["ratio"].asDouble();

		joint = gJoint;
	}
	else if (type == "wheel")
	{
		WheelJoint* wJoint = new WheelJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		wJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		wJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		wJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		wJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		wJoint->m_local_axis_a.x = jointValue["AxisA"]["x"].asDouble();
		wJoint->m_local_axis_a.y = jointValue["AxisA"]["y"].asDouble();

		wJoint->m_enable_motor = jointValue["enableMotor"].asBool();
		wJoint->m_max_motor_torque = jointValue["maxMotorTorque"].asDouble();

		wJoint->m_motor_speed = jointValue["motorSpeed"].asDouble();

		wJoint->m_frequency_hz = jointValue["frequencyHz"].asDouble();
		wJoint->m_damping_ratio = jointValue["dampingRatio"].asDouble();

		joint = wJoint;
	}
	else if (type == "weld")
	{
		WeldJoint* wJoint = new WeldJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		wJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		wJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		wJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		wJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		wJoint->m_reference_angle = jointValue["referenceAngle"].asDouble();
		wJoint->m_frequency_hz = jointValue["frequencyHz"].asDouble();
		wJoint->m_damping_ratio = jointValue["dampingRatio"].asDouble();

		joint = wJoint;
	}
	else if (type == "friction")
	{
		FrictionJoint* fJoint = new FrictionJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		fJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		fJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		fJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		fJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		fJoint->m_max_force = jointValue["m_max_force"].asDouble();
		fJoint->m_max_torque = jointValue["m_max_torque"].asDouble();

		joint = fJoint;
	}
	else if (type == "rope")
	{
		RopeJoint* rJoint = new RopeJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		rJoint->m_local_anchor_a.x = jointValue["anchorA"]["x"].asDouble();
		rJoint->m_local_anchor_a.y = jointValue["anchorA"]["y"].asDouble();
		rJoint->m_local_anchor_b.x = jointValue["anchorB"]["x"].asDouble();
		rJoint->m_local_anchor_b.y = jointValue["anchorB"]["y"].asDouble();

		rJoint->m_max_length = jointValue["maxLength"].asDouble();

		joint = rJoint;
	}
	else if (type == "motor")
	{
		MotorJoint* mJoint = new MotorJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		mJoint->m_max_force = jointValue["m_max_force"].asDouble();
		mJoint->m_max_torque = jointValue["m_max_torque"].asDouble();
		mJoint->m_correction_factor = jointValue["correctionFactor"].asDouble();

		joint = mJoint;
	}

	joint->m_name = jointValue["name"].asString();
	joint->m_collide_connected = jointValue["collideConnected"].asBool();

	return joint;
}

FileApapter::~FileApapter()
{
	Clear();
}

void FileApapter::Resolve(const std::string& filepath)
{
	Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	ToWorld(value["world"]);

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	int i = 0;
	Json::Value bodyValue = value["body"][i++];
	while (!bodyValue.isNull()) {
		Body* body = ToBody(bodyValue, dir);
		m_name_body_map.insert(std::make_pair(body->m_name, body));
		m_bodies.push_back(body);

		bodyValue = value["body"][i++];
	}

	i = 0;
	Json::Value jointValue = value["joint"][i++];
	while (!jointValue.isNull()) {
		Joint* joint = ToJoint(jointValue, m_bodies);
		m_name_joint_map.insert(std::make_pair(joint->m_name, joint));

		jointValue = value["joint"][i++];
	}
}

Body* FileApapter::QueryBody(size_t index) const
{
	if (index >= 0 && index < m_bodies.size())
		return m_bodies[index];
	else
		return NULL;
}

Body* FileApapter::QueryBody(const std::string& name) const
{
	std::map<std::string, Body*>::const_iterator itr
		= m_name_body_map.find(name);
	if (itr != m_name_body_map.end())
		return itr->second;
	else
		return NULL;
}

Joint* FileApapter::QueryJoint(const std::string& name) const
{
	std::map<std::string, Joint*>::const_iterator itr
		= m_name_joint_map.find(name);
	if (itr != m_name_joint_map.end())
		return itr->second;
	else
		return NULL;
}

void FileApapter::Clear()
{
	std::map<std::string, Body*>::iterator itr = m_name_body_map.begin();
	for ( ; itr != m_name_body_map.end(); ++itr)
		delete itr->second;
	m_name_body_map.clear();

	std::map<std::string, Joint*>::iterator itr2 = m_name_joint_map.begin();
	for ( ; itr2 != m_name_joint_map.end(); ++itr2)
		delete itr2->second;
	m_name_joint_map.clear();
}

}