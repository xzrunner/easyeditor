#include "Love2dCode.h"
#include "Context.h"
#include "StagePanel.h"

#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/FileHelper.h>

#include <easybuilder.h>
#include <easymodeling.h>
#include <easyshape.h>

#include <map>

namespace lua = ebuilder::lua;

namespace emodeling
{

Love2dCode::Love2dCode(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
	m_gen_header = new ebuilder::CodeGenerator;
	m_gen_load = new ebuilder::CodeGenerator;
	m_gen_update = new ebuilder::CodeGenerator;
	m_gen_draw = new ebuilder::CodeGenerator;

	m_gen.block(*m_gen_header);
	m_gen.line();
	m_gen.block(*m_gen_load);
	m_gen.line();
	m_gen.block(*m_gen_update);
	m_gen.line();
	m_gen.block(*m_gen_draw);
}

void Love2dCode::Resolve()
{
	ResolveHeader();
	ResolveLoad();
	ResolveUpdate();
	ResolveDraw();
}

void Love2dCode::ResolveHeader()
{
	ebuilder::CodeGenerator& gen = *m_gen_header;

	// actors = {}
	lua::assign(gen, "", "actors", "{}");
}

void Love2dCode::ResolveLoad()
{
	ebuilder::CodeGenerator& gen = *m_gen_load;

	// function love.load()
	lua::Function func(gen, "love.load", 0);

	ResolveLoadImages();
	gen.line();
	ResolveLoadWorld();
	gen.line();
	ResolveLoadBodies();
	gen.line();
	ResolveLoadJoints();
}

void Love2dCode::ResolveLoadImages()
{
	ebuilder::CodeGenerator& gen = *m_gen_load;

	// -- Load images.
	gen.line("-- Load images.");

	gen.line("images = {");
	gen.tab();

	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(ee::FetchAllVisitor<Body>(bodies));
	std::map<std::string, std::string> mapNamePath;
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		Body* body = bodies[i];
		std::string path = body->m_sprite->GetSymbol()->GetFilepath();
		std::string name = ee::FileHelper::GetFilename(path);
		mapNamePath.insert(std::make_pair(name, path));
	}

	std::map<std::string, std::string>::iterator itr = mapNamePath.begin();
	for ( ; itr != mapNamePath.end(); ++itr)
	{
		// "path"
		std::string path = "\""+ee::FileHelper::GetFilenameWithExtension(itr->second)+"\"";
		// love.graphics.newImage("path"),
		std::string str = lua::call("", "love.graphics.newImage", 1, path) + ",";
		// name = love.graphics.newImage("path"),
		lua::assign(gen, "", itr->first, str);
	}

	gen.detab();
	gen.line("}");
}

void Love2dCode::ResolveLoadWorld()
{
	ebuilder::CodeGenerator& gen = *m_gen_load;

	const World* world = Context::Instance()->world;

	// -- Create the world.
	gen.line("-- Create the world.");
	// world = love.physics.newWorld(x, y)
	std::string x = ee::StringHelper::ToString(world->gravity.x);
	std::string y = ee::StringHelper::ToString(-world->gravity.y); // love2d's origin is left-top
	lua::assign(gen, "", "world", lua::call("", "love.physics.newWorld", 2, x, y));
	// world:setAllowSleeping()
	lua::call("world", "setAllowSleeping", 1, world->allowSleep ? "true" : "false");
}

void Love2dCode::ResolveLoadBodies()
{
	ebuilder::CodeGenerator& gen = *m_gen_load;

	// -- Create actors.
	gen.line("-- Create actors.");
	// local shape
	gen.line("local shape");
	// local fixtures
	gen.line("local fixtures");
	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(ee::FetchAllVisitor<Body>(bodies));
	for (size_t i = 0, n = bodies.size(); i < n; ++i)
	{
		Body* body = bodies[i];

		gen.line();

		std::string name = ee::FileHelper::GetFilename(body->m_sprite->GetSymbol()->GetFilepath());

		// local actor = {}
		lua::assign(gen, "local", name, "{}");

		// actor.body = love.physics.newBody(world, x, y, "dynamic")
		std::string x = ee::StringHelper::ToString(body->m_sprite->GetPosition().x);
		std::string y = ee::StringHelper::ToString(-body->m_sprite->GetPosition().y);
		std::string type;
		switch (body->m_type)
		{
		case Body::e_static:
			type = "\"static\"";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			break;
		case Body::e_dynamic:
			type = "\"dynamic\"";
			break;
		case Body::e_kinematic:
			type = "\"kinematic\"";
			break;
		}
		std::string newBody = lua::call("", "love.physics.newBody", 4, "world", x, y, type);
		lua::assign(gen, "", name+".body", newBody);

		if (body->m_sprite->GetAngle() != 0)
		{
			// actor.body:setAngle(angle)
			std::string angle = ee::StringHelper::ToString(body->m_sprite->GetAngle());
			lua::call(gen, name+".body", "setAngle", 1, angle);
		}

		// actor.image = image
		std::string newImage = "images." + name;
		lua::assign(gen, "", name+".image", newImage);

		if (body->m_linear_damping != 0)
		{
			// actor.body:setLinearDamping(damping)
			std::string damping = ee::StringHelper::ToString(body->m_linear_damping);
			lua::call(gen, name+".body", "setLinearDamping", 1, damping);
		}

		if (body->m_angular_damping != 0)
		{
			// actor.body:setAngularDamping(damping)
			std::string damping = ee::StringHelper::ToString(body->m_angular_damping);
			lua::call(gen, name+".body", "setAngularDamping", 1, damping);
		}

		if (!body->m_allow_sleep)
		{
			// actor.body:setSleepingAllowed(false)
			lua::call(gen, name+".body", "setSleepingAllowed", 1, "false");
		}

		if (body->m_bullet)
		{
			// actor.body:setBullet(true)
			lua::call(gen, name+".body", "setBullet", 1, "true");
		}

		if (!body->m_active)
		{
			// actor.body:setActive(false)
			lua::call(gen, name+".body", "setActive", 1, "false");
		}

		if (body->m_gravity_scale != 1)
		{
			// actor.body:setGravityScale(scale)
			std::string scale = ee::StringHelper::ToString(body->m_gravity_scale);
			lua::call(gen, name+".body", "setGravityScale", 1, scale);
		}

		// resolve fixtures
		ResolveLoadFixtures(body);

		// table.insert(actors, actor)
		lua::call(gen, "", "table.insert", 2, "actors", name);
	}
}

void Love2dCode::ResolveLoadFixtures(Body* body)
{
	ebuilder::CodeGenerator& gen = *m_gen_load;

	for (size_t i = 0, n = body->m_fixtures.size(); i < n; ++i)
	{
		Fixture* fData = body->m_fixtures[i];

		std::string newShape;
		if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(fData->m_shape))
		{
			std::string radius = ee::StringHelper::ToString(circle->radius);
			if (circle->center.x == 0 && circle->center.y == 0)
			{
				// love.physics.newCircleShape(radius)
				newShape = lua::call("", "love.physics.newCircleShape", 1, radius);
			}
			else
			{
				// love.physics.newCircleShape(x, y, radius)
				std::string x = ee::StringHelper::ToString(circle->center.x);
				std::string y = ee::StringHelper::ToString(-circle->center.y);
				newShape = lua::call("", "love.physics.newCircleShape", 3, x, y, radius);
			}
		}
		else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(fData->m_shape))
		{
			// love.physics.newRectangleShape(x, y, w, h)
			std::string x = ee::StringHelper::ToString((rect->m_rect.xmax + rect->m_rect.xmin) * 0.5f),
				y = ee::StringHelper::ToString(-(rect->m_rect.ymax + rect->m_rect.ymin) * 0.5f);
			std::string w = ee::StringHelper::ToString(rect->m_rect.xmax - rect->m_rect.xmin),
				h = ee::StringHelper::ToString(rect->m_rect.ymax - rect->m_rect.ymin);
			newShape = lua::call("", "love.physics.newRectangleShape", 4, x, y, w, h);
		}
		else if (eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(fData->m_shape))
		{
			// love.physics.newPolygonShape(size, x0, y0, x1, y1, x2, y2, ... , xn, yn)
			const std::vector<sm::vec2>& vertices = polygon->GetVertices();
			std::string strParams;
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
			{
				std::string x = ee::StringHelper::ToString(vertices[i].x);
				std::string y = ee::StringHelper::ToString(-vertices[i].y);
				if (i == 0)
					strParams += x+", "+y;
				else
					strParams += ", "+x+", "+y;
			}
			newShape = lua::call("", "love.physics.newPolygonShape", 1, strParams);
		}
		else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(fData->m_shape))
		{
			// love.physics.newChainShape(loop, size, x0, y0, x1, y1, x2, y2, ... , xn, yn)
			std::string strParams = chain->IsClosed() ? "true" : "false";
			const std::vector<sm::vec2>& vertices = chain->GetVertices();
			std::string size = ee::StringHelper::ToString(vertices.size() * 2);
			strParams += ", "+size;
			for (size_t i = 0, n = vertices.size(); i < n; ++i)
			{
				std::string x = ee::StringHelper::ToString(vertices[i].x);
				std::string y = ee::StringHelper::ToString(-vertices[i].y);
				strParams += ", "+x+", "+y;
			}
			newShape = lua::call("", "love.physics.newChainShape", 1, strParams);
		}


		// local shape = newShape
		lua::assign(gen, "", "shape", newShape);
		// local fixture = love.physics.newFixture(body, shape)
		std::string sBody = ee::FileHelper::GetFilename(body->m_sprite->GetSymbol()->GetFilepath())+".body";
		lua::assign(gen, "", "fixture", lua::call("", "love.physics.newFixture", 2, sBody, "shape"));

		if (fData->m_density != 1)
		{
			// fixture:setDensity(density)
			std::string density = ee::StringHelper::ToString(fData->m_density);
			lua::call(gen, "fixture", "setDensity", 1, density);
		}
		if (fData->m_friction != 0.2f)
		{
			// fixture:setFriction(friction)
			std::string friction = ee::StringHelper::ToString(fData->m_friction);
			lua::call(gen, "fixture", "setFriction", 1, friction);
		}
		if (fData->m_restitution != 0)
		{
			// fixture:setRestitution(restitution)
			std::string restitution = ee::StringHelper::ToString(fData->m_restitution);
			lua::call(gen, "fixture", "setRestitution", 1, restitution);
		}
		if (fData->m_is_sensor)
		{
			// fixture:setSensor(true)
			lua::call(gen, "fixture", "setSensor", 1, "true");
		}
		if (fData->m_category_bits != 0x0001 || fData->m_mask_bits != 0xFFFF || fData->m_group_index != 0)
		{
			// fixture:setFilterData(categoryBits, maskBits, groupIndex)
			std::string categoryBits = ee::StringHelper::ToString(fData->m_category_bits);
			std::string maskBits = ee::StringHelper::ToString(fData->m_mask_bits);
			std::string groupIndex = ee::StringHelper::ToString(fData->m_group_index);
			lua::call(gen, "fixture", "setFilterData", 3, categoryBits, maskBits, groupIndex);
		}
	}
}

void Love2dCode::ResolveLoadJoints()
{
	ebuilder::CodeGenerator& gen = *m_gen_load;

	// -- Create joints.
	gen.line("-- Create joints.");

	std::vector<Joint*> joints;
	Context::Instance()->stage->traverseJoints(ee::FetchAllVisitor<Joint>(joints));
	// move gear joint to the end
	size_t iLast = joints.size() - 1;
	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		if (joints[i]->m_type == Joint::e_gearJoint)
		{
			Joint* tmp = joints[i];
			joints[i] = joints[iLast];
			joints[iLast] = tmp;
			--iLast;
		}
	}

	std::vector<Body*> bodies;
	Context::Instance()->stage->traverseBodies(ee::FetchAllVisitor<Body>(bodies));
	for (size_t i = 0, n = joints.size(); i < n; ++i)
	{
		gen.line();
		ResolveLoadJoint(joints, i, bodies);
	}
}

void Love2dCode::ResolveLoadJoint(const std::vector<Joint*>& joints, size_t index,
								  const std::vector<Body*>& bodies)
{
	Joint* j = joints[index];

	ebuilder::CodeGenerator& gen = *m_gen_load;

	std::string name = "joint" + ee::StringHelper::ToString(index);

	size_t iA = 0, iB = 0;
	for (size_t n = bodies.size(); iA < n; ++iA)
		if (bodies[iA] == j->m_body_a) break;
	for (size_t n = bodies.size(); iB < n; ++iB)
		if (bodies[iB] == j->m_body_b) break;

	std::string body1 = ee::StringHelper::ToString(iA + 1);
	body1 = "actors["+body1+"].body";

	std::string body2 = ee::StringHelper::ToString(iB + 1);
	body2 = "actors["+body2+"].body";

	std::string collideConnected = j->m_collide_connected ? "true" : "false";

	switch (j->m_type)
	{
	case Joint::e_revoluteJoint:
		{
			RevoluteJoint* joint = static_cast<RevoluteJoint*>(j);

			sm::vec2 anchorA = joint->GetWorldAnchorA();
			std::string x = ee::StringHelper::ToString(anchorA.x),
				y = ee::StringHelper::ToString(-anchorA.y);

			// local joint = love.physics.newRevoluteJoint(body1, body2, x, y, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newRevoluteJoint", 5, 
				body1, body2, x, y, collideConnected);
			lua::assign(gen, "local", name, newJoint);

			if (joint->m_enable_limit)
			{
				// joint:enableLimit(true)
				lua::call(gen, name, "enableLimit", 1, "true");
			}
			if (joint->m_lower_angle != 0 || joint->m_upper_angle != 0)
			{
				// joint:setLimits(lowerAngle, upperAngle)
				std::string lowerAngle = ee::StringHelper::ToString(joint->m_lower_angle),
					upperAngle = ee::StringHelper::ToString(joint->m_upper_angle);
				lua::call(gen, name, "setLimits", 2, lowerAngle, upperAngle);
			}
			if (joint->m_enable_motor)
			{
				// joint:enableMotor(true)
				lua::call(gen, name, "enableMotor", 1, "true");
			}
			if (joint->m_max_motor_torque != 0)
			{
				// joint:setMaxMotorTorque(maxMotorTorque)
				std::string maxMotorTorque = ee::StringHelper::ToString(joint->m_max_motor_torque);
				lua::call(gen, name, "setMaxMotorTorque", 1, maxMotorTorque);
			}
			if (joint->m_motor_speed != 0)
			{
				// joint:setMotorSpeed(motorSpeed)
				std::string motorSpeed = ee::StringHelper::ToString(joint->m_motor_speed);
				lua::call(gen, name, "setMotorSpeed", 1, motorSpeed);
			}
		}
		break;
	case Joint::e_prismaticJoint:
		{
			PrismaticJoint* joint = static_cast<PrismaticJoint*>(j);

			sm::vec2 anchorA = joint->GetWorldAnchorA(),
				anchorB = joint->GetWorldAnchorB();
			std::string xA = ee::StringHelper::ToString(anchorA.x),
				yA = ee::StringHelper::ToString(-anchorA.y);
			std::string xB = ee::StringHelper::ToString(anchorB.x),
				yB = ee::StringHelper::ToString(-anchorB.y);

			std::string ax = ee::StringHelper::ToString(joint->m_local_anchor_a.x),
				ay = ee::StringHelper::ToString(joint->m_local_anchor_a.y);

			// local joint = love.physics.newPrismaticJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPrismaticJoint", 9, 
				body1, body2, xA, yA, xB, yB, ax, ay, collideConnected);
			lua::assign(gen, "local", name, newJoint);

			if (joint->m_enable_limit)
			{
				// joint:enableLimit(true)
				lua::call(gen, name, "enableLimit", 1, "true");
			}
			if (joint->m_lower_translation != 0 || joint->m_upper_translation != 0)
			{
				// joint:setLimits(lowerAngle, upperAngle)
				std::string lowerTranslation = ee::StringHelper::ToString(joint->m_lower_translation),
					upperTranslation = ee::StringHelper::ToString(joint->m_upper_translation);
				lua::call(gen, name, "setLimits", 2, lowerTranslation, upperTranslation);
			}
			if (joint->m_enable_motor)
			{
				// joint:enableMotor(true)
				lua::call(gen, name, "enableMotor", 1, "true");
			}
			if (joint->m_max_motor_force != 0)
			{
				// joint:setMaxMotorForce(maxMotorForce)
				std::string maxMotorForce = ee::StringHelper::ToString(joint->m_max_motor_force);
				lua::call(gen, name, "setMaxMotorForce", 1, maxMotorForce);
			}
			if (joint->m_motor_speed != 0)
			{
				// joint:setMotorSpeed(motorSpeed)
				std::string motorSpeed = ee::StringHelper::ToString(joint->m_motor_speed);
				lua::call(gen, name, "setMotorSpeed", 1, motorSpeed);
			}
		}
		break;
	case Joint::e_distanceJoint:
		{
			DistanceJoint* joint = static_cast<DistanceJoint*>(j);

			sm::vec2 anchorA = joint->GetWorldAnchorA(),
				anchorB = joint->GetWorldAnchorB();
			std::string xA = ee::StringHelper::ToString(anchorA.x),
				yA = ee::StringHelper::ToString(-anchorA.y);
			std::string xB = ee::StringHelper::ToString(anchorB.x),
				yB = ee::StringHelper::ToString(-anchorB.y);

			// local joint = love.physics.newDistanceJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPrismaticJoint", 7, 
				body1, body2, xA, yA, xB, yB, collideConnected);
			lua::assign(gen, "local", name, newJoint);

			if (joint->m_frequency_hz != 0)
			{
				// joint:setFrequency(frequencyHz)
				std::string frequencyHz = ee::StringHelper::ToString(joint->m_frequency_hz);
				lua::call(gen, name, "setFrequency", 1, frequencyHz);
			}
			if (joint->m_damping_ratio != 0)
			{
				// joint:setDampingRatio(dampingRatio)
				std::string dampingRatio = ee::StringHelper::ToString(joint->m_damping_ratio);
				lua::call(gen, name, "setDampingRatio", 1, dampingRatio);
			}
		}
		break;
	case Joint::e_pulleyJoint:
		{
			PulleyJoint* joint = static_cast<PulleyJoint*>(j);

			std::string gxA = ee::StringHelper::ToString(joint->m_ground_anchor_a.x),
				gyA = ee::StringHelper::ToString(-joint->m_ground_anchor_a.y);
			std::string gxB = ee::StringHelper::ToString(joint->m_ground_anchor_b.x),
				gyB = ee::StringHelper::ToString(-joint->m_ground_anchor_b.y);

			sm::vec2 anchorA = joint->GetWorldAnchorA(),
				anchorB = joint->GetWorldAnchorB();
			std::string xA = ee::StringHelper::ToString(anchorA.x),
				yA = ee::StringHelper::ToString(-anchorA.y);
			std::string xB = ee::StringHelper::ToString(anchorB.x),
				yB = ee::StringHelper::ToString(-anchorB.y);

			std::string ratio = ee::StringHelper::ToString(joint->m_ratio);

			// local joint = love.physics.newPulleyJoint(body1, body2, gxA, gyA, gxB, gyB, xA, yA, xB, yB, ratio, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newPulleyJoint", 12, 
				body1, body2, gxA, gyA, gxB, gyB, xA, yA, xB, yB, ratio, collideConnected);
			lua::assign(gen, "local", name, newJoint);
		}
		break;
	case Joint::e_gearJoint:
		{
			GearJoint* joint = static_cast<GearJoint*>(j);

			size_t i1 = 0, i2 = 0;
			for (size_t n = joints.size(); i1 < n; ++i1)
				if (joints[i1] == joint->m_joint1)
					break;
			for (size_t n = joints.size(); i2 < n; ++i2)
				if (joints[i2] == joint->m_joint2)
					break;

			std::string joint1 = "joint"+ee::StringHelper::ToString(i1),
				joint2 = "joint"+ee::StringHelper::ToString(i2);

			std::string ratio = ee::StringHelper::ToString(joint->m_ratio);

			// local joint = love.physics.newGearJoint(joint1, joint2, ratio, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newGearJoint", 4, 
				joint1, joint2, ratio, collideConnected);
			lua::assign(gen, "local", name, newJoint);
		}
		break;
	case Joint::e_wheelJoint:
		{
			WheelJoint* joint = static_cast<WheelJoint*>(j);

			std::string xA = ee::StringHelper::ToString(joint->GetWorldAnchorA().x),
				yA = ee::StringHelper::ToString(-joint->GetWorldAnchorB().y);
			std::string ax = ee::StringHelper::ToString(joint->m_local_axis_a.x),
				ay = ee::StringHelper::ToString(-joint->m_local_axis_a.y);

			// local joint = love.physics.newWheelJoint(body1, body2, xA, yA, ax, ay, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newWheelJoint", 7, 
				body1, body2, xA, yA, ax, ay, collideConnected);
			lua::assign(gen, "local", name, newJoint);

			if (joint->m_enable_motor)
			{
				// joint:enableMotor(true)
				lua::call(gen, name, "enableMotor", 1, "true");
			}
			if (joint->m_max_motor_torque != 0)
			{
				// joint:setMaxMotorTorque(maxMotorForce)
				std::string maxMotorTorque = ee::StringHelper::ToString(joint->m_max_motor_torque);
				lua::call(gen, name, "setMaxMotorTorque", 1, maxMotorTorque);
			}
			if (joint->m_motor_speed != 0)
			{
				// joint:setMotorSpeed(motorSpeed)
				std::string motorSpeed = ee::StringHelper::ToString(joint->m_motor_speed);
				lua::call(gen, name, "setMotorSpeed", 1, motorSpeed);
			}
			if (joint->m_frequency_hz != 2.0f)
			{
				// joint:setSpringFrequency(frequencyHz)
				std::string frequencyHz = ee::StringHelper::ToString(joint->m_frequency_hz);
				lua::call(gen, name, "setSpringFrequency", 1, frequencyHz);
			}
			if (joint->m_damping_ratio != 0.7f)
			{
				// joint:setSpringDampingRatio(dampingRatio)
				std::string dampingRatio = ee::StringHelper::ToString(joint->m_damping_ratio);
				lua::call(gen, name, "setSpringDampingRatio", 1, dampingRatio);
			}
		}
		break;
	case Joint::e_weldJoint:
		{
			WeldJoint* joint = static_cast<WeldJoint*>(j);

			sm::vec2 anchorA = joint->GetWorldAnchorA(),
				anchorB = joint->GetWorldAnchorB();
			std::string xA = ee::StringHelper::ToString(anchorA.x),
				yA = ee::StringHelper::ToString(-anchorA.y);
			std::string xB = ee::StringHelper::ToString(anchorB.x),
				yB = ee::StringHelper::ToString(-anchorB.y);

			// local joint = love.physics.newWeldJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newWeldJoint", 7, 
				body1, body2, xA, yA, xB, yB, collideConnected);
			lua::assign(gen, "local", name, newJoint);

			if (joint->m_frequency_hz != 0)
			{
				// joint:setFrequency(frequencyHz)
				std::string frequencyHz = ee::StringHelper::ToString(joint->m_frequency_hz);
				lua::call(gen, name, "setFrequency", 1, frequencyHz);
			}
			if (joint->m_damping_ratio != 0)
			{
				// joint:setDampingRatio(dampingRatio)
				std::string dampingRatio = ee::StringHelper::ToString(joint->m_damping_ratio);
				lua::call(gen, name, "setDampingRatio", 1, dampingRatio);
			}
		}
		break;
	case Joint::e_frictionJoint:
		{
			FrictionJoint* joint = static_cast<FrictionJoint*>(j);

			sm::vec2 anchorA = joint->GetWorldAnchorA(),
				anchorB = joint->GetWorldAnchorB();
			std::string xA = ee::StringHelper::ToString(anchorA.x),
				yA = ee::StringHelper::ToString(-anchorA.y);
			std::string xB = ee::StringHelper::ToString(anchorB.x),
				yB = ee::StringHelper::ToString(-anchorB.y);

			// local joint = love.physics.newFrictionJoint(body1, body2, xA, yA, xB, yB, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newFrictionJoint", 7, 
				body1, body2, xA, yA, xB, yB, collideConnected);
			lua::assign(gen, "local", name, newJoint);

			if (joint->m_max_force != 0)
			{
				// joint:setMaxForce(maxForce)
				std::string maxForce = ee::StringHelper::ToString(joint->m_max_force);
				lua::call(gen, name, "setMaxForce", 1, maxForce);
			}
			if (joint->m_max_torque != 0)
			{
				// joint:setMaxTorque(maxTorque)
				std::string maxTorque = ee::StringHelper::ToString(joint->m_max_torque);
				lua::call(gen, name, "setMaxTorque", 1, maxTorque);
			}
		}
		break;
	case Joint::e_ropeJoint:
		{
			RopeJoint* joint = static_cast<RopeJoint*>(j);

			sm::vec2 anchorA = joint->GetWorldAnchorA(),
				anchorB = joint->GetWorldAnchorB();
			std::string xA = ee::StringHelper::ToString(anchorA.x),
				yA = ee::StringHelper::ToString(-anchorA.y);
			std::string xB = ee::StringHelper::ToString(anchorB.x),
				yB = ee::StringHelper::ToString(-anchorB.y);

			std::string maxLength = ee::StringHelper::ToString(joint->m_max_length);

			// local joint = love.physics.newRopeJoint(body1, body2, xA, yA, xB, yB, maxLength, collideConnected)
			std::string newJoint = lua::call("", "love.physics.newFrictionJoint", 8, 
				body1, body2, xA, yA, xB, yB, maxLength, collideConnected);
			lua::assign(gen, "local", name, newJoint);

		}
		break;
	case Joint::e_motorJoint:
		{
		}
		break;
	}
}

void Love2dCode::ResolveUpdate()
{
	ebuilder::CodeGenerator& gen = *m_gen_update;

	// function love.load()
	lua::Function func(gen, "love.update", 1, "dt");

	// -- Update the world.
	gen.line("-- Update the world.");
	// world:update(dt)
	lua::call(gen, "world", "update", 1, "dt");
}

void Love2dCode::ResolveDraw()
{
	ebuilder::CodeGenerator& gen = *m_gen_draw;

	// function love.draw()
	lua::Function func(gen, "love.draw", 0);

	// love.graphics.push()
	lua::call(gen, "", "love.graphics.push", 0);
	// love.graphics.translate(400, 240)
	lua::call(gen, "", "love.graphics.translate", 2, "400", "240");

	{
		// for i, v in ipairs(actors) do
		lua::For _for(gen, "i, v in ipairs(actors)");
		// love.graphics.draw(v.image, v.body:getX(), v.body:getY(), v.body:getAngle(), 1, 1, ox, oy)
		std::string ox = "v.image:getWidth() * 0.5",
			oy = "v.image:getHeight() * 0.5";
		lua::call(gen, "", "love.graphics.draw", 8, "v.image", 
			"v.body:getX()", "v.body:getY()", "v.body:getAngle()", "1", "1", ox, oy);
	}

	// love.graphics.pop()
	lua::call(gen, "", "love.graphics.pop", 0);
}

}