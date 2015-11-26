
#include "FileIO.h"
#include "Context.h"

#include "StagePanel.h"

#include <easyshape.h>

namespace emodeling
{

void FileIO::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Context* context = Context::Instance();

	std::vector<libmodeling::Body*> bodies;

	delete context->world;
	context->world = libmodeling::FileApapter::j2World(value["world"]);

	std::string dir = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value bodyValue = value["body"][i++];
	while (!bodyValue.isNull()) {
		libmodeling::Body* body = libmodeling::FileApapter::j2bBody(bodyValue, dir);
		d2d::InsertSpriteSJ::Instance()->Insert(body->sprite);
		bodies.push_back(body);

		bodyValue = value["body"][i++];
	}

	i = 0;
	std::vector<libmodeling::Joint*> joints;
	Json::Value jointValue = value["joint"][i++];
	while (!jointValue.isNull()) {
		libmodeling::Joint* joint = libmodeling::FileApapter::j2bJoint(jointValue, bodies);
		joints.push_back(joint);
		context->stage->insertJoint(joint);
		jointValue = value["joint"][i++];
	}

	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		if (joints[i]->type == libmodeling::Joint::e_gearJoint)
		{
			libmodeling::GearJoint* joint = static_cast<libmodeling::GearJoint*>(joints[i]);
			joint->joint1 = joints[value["joint"][i]["joint1"].asInt()];
			joint->joint2 = joints[value["joint"][i]["joint2"].asInt()];
		}
	}

	context->library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
	d2d::ResetViewportSJ::Instance()->Reset();
}

void FileIO::store(const char* filename)
{
	std::vector<libmodeling::Body*> bodies;
	Context::Instance()->stage->traverseBodies(d2d::FetchAllVisitor<libmodeling::Body>(bodies));

	std::vector<libmodeling::Joint*> joints;
	Context::Instance()->stage->traverseJoints(d2d::FetchAllVisitor<libmodeling::Joint>(joints));

	Json::Value value;

	std::string dir = d2d::FilenameTools::getFileDir(filename);

	value["world"] = b2j(Context::Instance()->world);

	std::map<libmodeling::Body*, int> bodyIndexMap;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		bodyIndexMap[bodies[i]] = i;
		value["body"][i] = b2j(bodies[i], dir);
	}

	std::map<libmodeling::Joint*, int> jointIndexMap;
	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		jointIndexMap[joints[i]] = i;
		value["joint"][i] = b2j(joints[i], bodyIndexMap);
	}

	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		if (joints[i]->type == libmodeling::Joint::e_gearJoint)
		{
			libmodeling::GearJoint* joint = static_cast<libmodeling::GearJoint*>(joints[i]);
			value["joint"][i]["joint1"] = jointIndexMap.find(joint->joint1)->second;
			value["joint"][i]["joint2"] = jointIndexMap.find(joint->joint2)->second;
		}
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

Json::Value FileIO::b2j(const libmodeling::World* world)
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

Json::Value FileIO::b2j(const libmodeling::Body* body, const std::string& dlg)
{
	Json::Value value;

	value["name"] = body->name.ToStdString();

	value["filepath"] = d2d::FilenameTools::getRelativePath(dlg, 
		body->sprite->GetSymbol().GetFilepath()).ToStdString();

	value["type"] = body->type;
	switch (body->type)
	{
	case libmodeling::Body::e_static:
		value["type"].setComment("//static", Json::commentAfterOnSameLine);
		break;
	case libmodeling::Body::e_dynamic:
		value["type"].setComment("//dynamic", Json::commentAfterOnSameLine);
		break;
	case libmodeling::Body::e_kinematic:
		value["type"].setComment("//kinematic", Json::commentAfterOnSameLine);
		break;
	}

	value["linearDamping"] = body->linearDamping;

	value["angularDamping"] = body->angularDamping;

	value["allowSleep"] = body->allowSleep;

	value["bullet"] = body->bullet;

	value["active"] = body->active;

	value["gravityScale"] = body->gravityScale;

	value["position"]["x"] = body->sprite->GetPosition().x;
	value["position"]["y"] = body->sprite->GetPosition().y;

	value["angle"] = body->sprite->GetAngle();

	for (size_t i = 0, n = body->fixtures.size(); i < n; ++i)
		value["fixture"][i] = b2j(body->fixtures[i]);

	return value;
}

Json::Value FileIO::b2j(const libmodeling::Fixture* fixture)
{
	Json::Value value;

	value["name"] = fixture->name.ToStdString();
	value["density"] = fixture->density;
	value["friction"] = fixture->friction;
	value["restitution"] = fixture->restitution;
	value["isSensor"] = fixture->isSensor;
	value["categoryBits"] = fixture->categoryBits;
	value["maskBits"] = fixture->maskBits;
	value["groupIndex"] = fixture->groupIndex;

	// todo
//	value["shape"] = libshape::FileIO::StoreShape(fixture->shape);

	return value;
}

Json::Value FileIO::b2j(libmodeling::Joint* joint, const std::map<libmodeling::Body*, int>& bodyIndexMap)
{
	Json::Value value;

	value["name"] = joint->m_name.ToStdString();
	
	std::map<libmodeling::Body*, int>::const_iterator itrA = bodyIndexMap.find(joint->bodyA);
	assert(itrA != bodyIndexMap.end());
	value["bodyA"] = itrA->second;

	std::map<libmodeling::Body*, int>::const_iterator itrB = bodyIndexMap.find(joint->bodyB);
	assert(itrB != bodyIndexMap.end());
	value["bodyB"] = itrB->second;

	value["collideConnected"] = joint->collideConnected;

	switch (joint->type)
	{
	case libmodeling::Joint::e_revoluteJoint:
		{
			value["type"] = "revolute";

			libmodeling::RevoluteJoint* rJoint = static_cast<libmodeling::RevoluteJoint*>(joint);

			value["anchorA"]["x"] = rJoint->localAnchorA.x;
			value["anchorA"]["y"] = rJoint->localAnchorA.y;
			value["anchorB"]["x"] = rJoint->localAnchorB.x;
			value["anchorB"]["y"] = rJoint->localAnchorB.y;

			value["refAngle"] = rJoint->referenceAngle;

			value["enableLimit"] = rJoint->enableLimit;
			value["lowerAngle"] = rJoint->lowerAngle;
			value["upperAngle"] = rJoint->upperAngle;

			value["enableMotor"] = rJoint->enableMotor;
			value["maxMotorTorque"] = rJoint->maxMotorTorque;

			value["motorSpeed"] = rJoint->motorSpeed;
		}
		break;
	case libmodeling::Joint::e_prismaticJoint:
		{
			value["type"] = "prismatic";

			libmodeling::PrismaticJoint* rJoint = static_cast<libmodeling::PrismaticJoint*>(joint);

			value["anchorA"]["x"] = rJoint->localAnchorA.x;
			value["anchorA"]["y"] = rJoint->localAnchorA.y;
			value["anchorB"]["x"] = rJoint->localAnchorB.x;
			value["anchorB"]["y"] = rJoint->localAnchorB.y;

			value["localAxisA"]["x"] = rJoint->localAxisA.x;
			value["localAxisA"]["y"] = rJoint->localAxisA.y;

			value["refAngle"] = rJoint->referenceAngle;

			value["enableLimit"] = rJoint->enableLimit;
			value["lowerTranslation"] = rJoint->lowerTranslation;
			value["upperTranslation"] = rJoint->upperTranslation;

			value["enableMotor"] = rJoint->enableMotor;
			value["maxMotorForce"] = rJoint->maxMotorForce;

			value["motorSpeed"] = rJoint->motorSpeed;
		}
		break;
	case libmodeling::Joint::e_distanceJoint:
		{
			value["type"] = "distance";

			libmodeling::DistanceJoint* dJoint = static_cast<libmodeling::DistanceJoint*>(joint);

			value["anchorA"]["x"] = dJoint->localAnchorA.x;
			value["anchorA"]["y"] = dJoint->localAnchorA.y;
			value["anchorB"]["x"] = dJoint->localAnchorB.x;
			value["anchorB"]["y"] = dJoint->localAnchorB.y;

			value["frequencyHz"] = dJoint->frequencyHz;
			value["dampingRatio"] = dJoint->dampingRatio;
		}
		break;
	case libmodeling::Joint::e_pulleyJoint:
		{
			value["type"] = "pulley";

			libmodeling::PulleyJoint* pJoint = static_cast<libmodeling::PulleyJoint*>(joint);

			value["ratio"] = pJoint->ratio;
		}
		break;
	case libmodeling::Joint::e_gearJoint:
		{
			value["type"] = "gear";

			libmodeling::GearJoint* gJoint = static_cast<libmodeling::GearJoint*>(joint);

			value["ratio"] = gJoint->ratio;
		}
		break;
	case libmodeling::Joint::e_wheelJoint:
		{
			value["type"] = "wheel";

			libmodeling::WheelJoint* wJoint = static_cast<libmodeling::WheelJoint*>(joint);

			value["anchorA"]["x"] = wJoint->localAnchorA.x;
			value["anchorA"]["y"] = wJoint->localAnchorA.y;
			value["anchorB"]["x"] = wJoint->localAnchorB.x;
			value["anchorB"]["y"] = wJoint->localAnchorB.y;

			float len = d2d::Math::getDistance(wJoint->localAxisA, d2d::Vector());
			value["AxisA"]["x"] = wJoint->localAxisA.x / len;
			value["AxisA"]["y"] = wJoint->localAxisA.y / len;

			value["enableMotor"] = wJoint->enableMotor;
			value["maxMotorTorque"] = wJoint->maxMotorTorque;

			value["motorSpeed"] = wJoint->motorSpeed;

			value["frequencyHz"] = wJoint->frequencyHz;
			value["dampingRatio"] = wJoint->dampingRatio;
		}
		break;
	case libmodeling::Joint::e_weldJoint:
		{
			value["type"] = "weld";

			libmodeling::WeldJoint* wJoint = static_cast<libmodeling::WeldJoint*>(joint);

			value["anchorA"]["x"] = wJoint->localAnchorA.x;
			value["anchorA"]["y"] = wJoint->localAnchorA.y;
			value["anchorB"]["x"] = wJoint->localAnchorB.x;
			value["anchorB"]["y"] = wJoint->localAnchorB.y;

			value["referenceAngle"] = wJoint->dampingRatio;
			value["frequencyHz"] = wJoint->frequencyHz;
			value["dampingRatio"] = wJoint->dampingRatio;
		}
		break;
	case libmodeling::Joint::e_frictionJoint:
		{
			value["type"] = "friction";

			libmodeling::FrictionJoint* fJoint = static_cast<libmodeling::FrictionJoint*>(joint);

			value["anchorA"]["x"] = fJoint->localAnchorA.x;
			value["anchorA"]["y"] = fJoint->localAnchorA.y;
			value["anchorB"]["x"] = fJoint->localAnchorB.x;
			value["anchorB"]["y"] = fJoint->localAnchorB.y;

			value["maxForce"] = fJoint->maxForce;
			value["maxTorque"] = fJoint->maxTorque;
		}
		break;
	case libmodeling::Joint::e_ropeJoint:
		{
			value["type"] = "rope";

			libmodeling::RopeJoint* rJoint = static_cast<libmodeling::RopeJoint*>(joint);

			value["anchorA"]["x"] = rJoint->localAnchorA.x;
			value["anchorA"]["y"] = rJoint->localAnchorA.y;
			value["anchorB"]["x"] = rJoint->localAnchorB.x;
			value["anchorB"]["y"] = rJoint->localAnchorB.y;

			value["maxLength"] = rJoint->maxLength;
		}
		break;
	case libmodeling::Joint::e_motorJoint:
		{
			value["type"] = "motor";

			libmodeling::MotorJoint* mJoint = static_cast<libmodeling::MotorJoint*>(joint);

			value["maxForce"] = mJoint->maxForce;
			value["maxTorque"] = mJoint->maxTorque;
			value["correctionFactor"] = mJoint->correctionFactor;
		}
		break;
	}

	return value;
}

}