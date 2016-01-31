
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

#include <easyshape.h>

using namespace libmodeling;

World* FileApapter::j2World(const Json::Value& worldValue)
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

Body* FileApapter::j2bBody(const Json::Value& bodyValue, const std::string& dlg)
{
	std::string filepath = bodyValue["filepath"].asString();
	filepath = ee::FileHelper::GetAbsolutePath(dlg, filepath);

	ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
	symbol->Release();

	ee::Vector pos;
	pos.x = bodyValue["position"]["x"].asDouble();
	pos.y = bodyValue["position"]["y"].asDouble();
	float angle = bodyValue["angle"].asDouble();

	sprite->SetTransform(pos, angle);

	Body* body = new Body;
	body->name = bodyValue["name"].asString();
	body->type = Body::Type(bodyValue["type"].asInt());
	body->linearDamping = bodyValue["linearDamping"].asDouble();
	body->angularDamping = bodyValue["angularDamping"].asDouble();
	body->allowSleep = bodyValue["allowSleep"].asBool();
	body->bullet = bodyValue["bullet"].asBool();
	body->active = bodyValue["active"].asBool();
	body->gravityScale = bodyValue["gravityScale"].asDouble();
	body->sprite = sprite;

	sprite->SetUserData(body);

	int i = 0;
	Json::Value fixtureValue = bodyValue["fixture"][i++];
	while (!fixtureValue.isNull()) {
		Fixture* fixture = j2bFixture(fixtureValue);
		fixture->body = body;
		body->fixtures.push_back(fixture);

		fixtureValue = bodyValue["fixture"][i++];
	}

	return body;
}

Fixture* FileApapter::j2bFixture(const Json::Value& fixtureValue)
{
	Fixture* fixture = new Fixture;

	fixture->name = fixtureValue["name"].asString();

	fixture->density = fixtureValue["density"].asDouble();
	fixture->friction = fixtureValue["friction"].asDouble();
	fixture->restitution = fixtureValue["restitution"].asDouble();
	fixture->isSensor = fixtureValue["isSensor"].asBool();
	fixture->categoryBits = fixtureValue["categoryBits"].asInt();
	fixture->maskBits = fixtureValue["maskBits"].asInt();
	fixture->groupIndex = fixtureValue["groupIndex"].asInt();

	// todo
//	fixture->shape = libshape::FileIO::LoadShape(fixtureValue["shape"]);

	return fixture;
}

Joint* FileApapter::j2bJoint(const Json::Value& jointValue,
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

		rJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		rJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		rJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		rJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		rJoint->referenceAngle = jointValue["refAngle"].asDouble();

		rJoint->enableLimit = jointValue["enableLimit"].asBool();
		rJoint->lowerAngle = jointValue["lowerAngle"].asDouble();
		rJoint->upperAngle = jointValue["upperAngle"].asDouble();

		rJoint->enableMotor = jointValue["enableMotor"].asBool();
		rJoint->maxMotorTorque = jointValue["maxMotorTorque"].asDouble();

		rJoint->motorSpeed = jointValue["motorSpeed"].asDouble();

		joint = rJoint;
	}
	else if (type == "prismatic")
	{
		PrismaticJoint* pJoint = new PrismaticJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		pJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		pJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		pJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		pJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		pJoint->localAxisA.x = jointValue["localAxisA"]["x"].asDouble();
		pJoint->localAxisA.y = jointValue["localAxisA"]["y"].asDouble();

		pJoint->referenceAngle = jointValue["refAngle"].asDouble();

		pJoint->enableLimit = jointValue["enableLimit"].asBool();
		pJoint->lowerTranslation = jointValue["lowerTranslation"].asDouble();
		pJoint->upperTranslation = jointValue["upperTranslation"].asDouble();

		pJoint->enableMotor = jointValue["enableMotor"].asBool();
		pJoint->maxMotorForce = jointValue["maxMotorForce"].asDouble();

		pJoint->motorSpeed = jointValue["motorSpeed"].asDouble();

		joint = pJoint;
	}
	else if (type == "distance")
	{
		DistanceJoint* dJoint = new DistanceJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		dJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		dJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		dJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		dJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		dJoint->frequencyHz = jointValue["frequencyHz"].asDouble();
		dJoint->dampingRatio = jointValue["dampingRatio"].asDouble();

		joint = dJoint;
	}
	else if (type == "pulley")
	{
		PulleyJoint* pJoint = new PulleyJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		pJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		pJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		pJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		pJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		pJoint->groundAnchorA.x = jointValue["groundAnchorA"]["x"].asDouble();
		pJoint->groundAnchorA.y = jointValue["groundAnchorA"]["y"].asDouble();
		pJoint->groundAnchorB.x = jointValue["groundAnchorB"]["x"].asDouble();
		pJoint->groundAnchorB.y = jointValue["groundAnchorB"]["y"].asDouble();

		pJoint->ratio = jointValue["ratio"].asDouble();

		joint = pJoint;
	}
	else if (type == "gear")
	{
		GearJoint* gJoint = new GearJoint(bodies[bodyIndexA], bodies[bodyIndexB], NULL, NULL);

		gJoint->ratio = jointValue["ratio"].asDouble();

		joint = gJoint;
	}
	else if (type == "wheel")
	{
		WheelJoint* wJoint = new WheelJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		wJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		wJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		wJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		wJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		wJoint->localAxisA.x = jointValue["AxisA"]["x"].asDouble();
		wJoint->localAxisA.y = jointValue["AxisA"]["y"].asDouble();

		wJoint->enableMotor = jointValue["enableMotor"].asBool();
		wJoint->maxMotorTorque = jointValue["maxMotorTorque"].asDouble();

		wJoint->motorSpeed = jointValue["motorSpeed"].asDouble();

		wJoint->frequencyHz = jointValue["frequencyHz"].asDouble();
		wJoint->dampingRatio = jointValue["dampingRatio"].asDouble();

		joint = wJoint;
	}
	else if (type == "weld")
	{
		WeldJoint* wJoint = new WeldJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		wJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		wJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		wJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		wJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		wJoint->referenceAngle = jointValue["referenceAngle"].asDouble();
		wJoint->frequencyHz = jointValue["frequencyHz"].asDouble();
		wJoint->dampingRatio = jointValue["dampingRatio"].asDouble();

		joint = wJoint;
	}
	else if (type == "friction")
	{
		FrictionJoint* fJoint = new FrictionJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		fJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		fJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		fJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		fJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		fJoint->maxForce = jointValue["maxForce"].asDouble();
		fJoint->maxTorque = jointValue["maxTorque"].asDouble();

		joint = fJoint;
	}
	else if (type == "rope")
	{
		RopeJoint* rJoint = new RopeJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		rJoint->localAnchorA.x = jointValue["anchorA"]["x"].asDouble();
		rJoint->localAnchorA.y = jointValue["anchorA"]["y"].asDouble();
		rJoint->localAnchorB.x = jointValue["anchorB"]["x"].asDouble();
		rJoint->localAnchorB.y = jointValue["anchorB"]["y"].asDouble();

		rJoint->maxLength = jointValue["maxLength"].asDouble();

		joint = rJoint;
	}
	else if (type == "motor")
	{
		MotorJoint* mJoint = new MotorJoint(bodies[bodyIndexA], bodies[bodyIndexB]);

		mJoint->maxForce = jointValue["maxForce"].asDouble();
		mJoint->maxTorque = jointValue["maxTorque"].asDouble();
		mJoint->correctionFactor = jointValue["correctionFactor"].asDouble();

		joint = mJoint;
	}

	joint->m_name = jointValue["name"].asString();
	joint->collideConnected = jointValue["collideConnected"].asBool();

	return joint;
}

FileApapter::~FileApapter()
{
	clear();
}

void FileApapter::resolve(const wxString& filepath)
{
	clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	j2World(value["world"]);

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	int i = 0;
	Json::Value bodyValue = value["body"][i++];
	while (!bodyValue.isNull()) {
		Body* body = j2bBody(bodyValue, dir);
		m_nameBodyMap.insert(std::make_pair(body->name, body));
		m_bodies.push_back(body);

		bodyValue = value["body"][i++];
	}

	i = 0;
	Json::Value jointValue = value["joint"][i++];
	while (!jointValue.isNull()) {
		Joint* joint = j2bJoint(jointValue, m_bodies);
		m_nameJointMap.insert(std::make_pair(joint->m_name, joint));

		jointValue = value["joint"][i++];
	}
}

Body* FileApapter::queryBody(size_t index) const
{
	if (index >= 0 && index < m_bodies.size())
		return m_bodies[index];
	else
		return NULL;
}

Body* FileApapter::queryBody(const wxString& name) const
{
	std::map<wxString, Body*>::const_iterator itr
		= m_nameBodyMap.find(name);
	if (itr != m_nameBodyMap.end())
		return itr->second;
	else
		return NULL;
}

Joint* FileApapter::queryJoint(const wxString& name) const
{
	std::map<wxString, Joint*>::const_iterator itr
		= m_nameJointMap.find(name);
	if (itr != m_nameJointMap.end())
		return itr->second;
	else
		return NULL;
}

void FileApapter::clear()
{
	std::map<wxString, Body*>::iterator itr = m_nameBodyMap.begin();
	for ( ; itr != m_nameBodyMap.end(); ++itr)
		delete itr->second;
	m_nameBodyMap.clear();

	std::map<wxString, Joint*>::iterator itr2 = m_nameJointMap.begin();
	for ( ; itr2 != m_nameJointMap.end(); ++itr2)
		delete itr2->second;
	m_nameJointMap.clear();
}
